#include "wallet.h"
#include "app_wrapper.h"

#include "key.h"
#include "base58.h"
#include "univalue/include/univalue.h"
#include "rpcclient.h"
#include "rpcserver.h"

#include <boost/algorithm/string.hpp>


static std::list<std::string> _invokeRpc(const std::string& args)
{
  std::list<std::string> resultList;
  UniValue result;
  std::vector<std::string> vArgs;

  try {
    boost::split(vArgs, args, boost::is_any_of(" \t"));

    const auto strMethod = vArgs[0];
    vArgs.erase(vArgs.begin());

    // Handle empty strings the same way as CLI
    for (size_t i = 0; i < vArgs.size(); i++) {
        if (vArgs[i] == "\"\"") {
            vArgs[i] = "";
        }
    }

    UniValue params = RPCConvertValues(strMethod, vArgs);

    if (nullptr == tableRPC[strMethod]) {
      resultList.push_back("Error");
      resultList.push_back("No such a Jni Api " + strMethod);
      return resultList;
    }

    std::string res;
    rpcfn_type method = tableRPC[strMethod]->actor;
    result = (*method)(params, false);

    result.feedStringList(resultList);
  }
  catch(const UniValue &objError) {
    result = objError;

    resultList.push_back("Error");
    resultList.push_back(find_value(result.get_obj(), "message").get_str());
  }
  catch(...) {
    resultList.push_back("Error");
    resultList.push_back("Unknown exception!");
  }

  return resultList;
}

void execute(const std::string& _nettype, const std::string& args, std::list<std::string>& result)
{
  CBaseChainParams::Network netType;
  if ("main" == _nettype) {
    netType = CBaseChainParams::MAIN;
  }
  else if ("testnet" == _nettype) {
    netType = CBaseChainParams::TESTNET;
  }
  else {
    result = {"Error", "Unknown net type!"};
  }

  AppWrapper aw(netType);
  result = _invokeRpc(args);
}
