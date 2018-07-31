#pragma once

#include "test/test_bitcoin.h"
#include "wallet/wallet.h"
#include "txdb.h"

class AppWrapper
{
public:
  AppWrapper(const CBaseChainParams::Network& netType);
  ~AppWrapper();

  bool valid() { return valid_;}

private:
  bool valid_ = false;

  BasicTestingSetup bs;


  CCoinsViewDB *pcoinsdbview;
  CWallet* pwalletMain;
  ZCJoinSplit *pzcashParams;

  boost::filesystem::path pathTemp;
  boost::thread_group threadGroup;
};
