#include "app_wrapper.h"
#include "chainparams.h"
#include "rpcserver.h"
#include "consensus/validation.h"
#include "net.h"

extern CBlockTreeDB *pblocktree;
extern CCoinsViewCache *pcoinsTip;
extern int nScriptCheckThreads;

bool InitBlockIndex();
void RegisterNodeSignals(CNodeSignals& nodeSignals);
void UnregisterNodeSignals(CNodeSignals& nodeSignals);
void UnloadBlockIndex();
void ThreadScriptCheck();

AppWrapper::AppWrapper(const CBaseChainParams::Network& netType): bs(netType)
{
  //copy from TestingSetup::TestingSetup

#ifdef ENABLE_WALLET
  bitdb.MakeMock();
#endif
  ClearDatadirCache();
  pathTemp = GetTempPath() / strprintf("test_bitcoin_%lu_%i", (unsigned long)GetTime(), (int)(GetRand(100000)));
  boost::filesystem::create_directories(pathTemp);
  mapArgs["-datadir"] = pathTemp.string();
  pblocktree = new CBlockTreeDB(1 << 20, true);
  pcoinsdbview = new CCoinsViewDB(1 << 23, true);
  pcoinsTip = new CCoinsViewCache(pcoinsdbview);
  InitBlockIndex();
#ifdef ENABLE_WALLET
  bool fFirstRun;
  pwalletMain = new CWallet("wallet.dat");
  pwalletMain->LoadWallet(fFirstRun);
  RegisterValidationInterface(pwalletMain);
#endif
  nScriptCheckThreads = 3;
  for (int i=0; i < nScriptCheckThreads-1; i++)
    threadGroup.create_thread(&ThreadScriptCheck);
  RegisterNodeSignals(GetNodeSignals());

  valid_ = true;
}
AppWrapper::~AppWrapper()
{
  UnregisterNodeSignals(GetNodeSignals());
  threadGroup.interrupt_all();
  threadGroup.join_all();
#ifdef ENABLE_WALLET
  UnregisterValidationInterface(pwalletMain);
  delete pwalletMain;
  pwalletMain = NULL;
#endif
  UnloadBlockIndex();
  delete pcoinsTip;
  delete pcoinsdbview;
  delete pblocktree;
#ifdef ENABLE_WALLET
  bitdb.Flush(true);
  bitdb.Reset();
#endif
  boost::filesystem::remove_all(pathTemp);
}
