#include <stdio.h>
#include "gtest/gtest.h"
#include "wallet.h"
#include <dlfcn.h>
#include <stdlib.h>
#include <list>


typedef void (*execute_t)(const std::string& _nettype, const std::string& args, std::list<std::string>& result);

class OKWalletTest : public ::testing::Test {
public:
  static void SetUpTestCase() {
    const auto& cwd = _get_current_dir();
    ASSERT_FALSE(cwd.empty());
#ifdef __APPLE__
    const auto& mod_path = cwd + "/libzen.dylib";
#elif __linux__
    const auto& mod_path = cwd + "/libzen.so";
#endif

    wlt_mod = dlopen(mod_path.c_str(), RTLD_LAZY);
    execute = (execute_t)dlsym(wlt_mod, "execute");
  }

  static void TearDownTestCase() {
    if (NULL != wlt_mod) {
      dlclose(wlt_mod);
      wlt_mod = NULL;
    }
  }

  static std::string _get_current_dir()
  {
    auto* buf = getcwd(NULL, 0);
    if (nullptr == buf) {
      return "";
    }

    std::string res(buf);
    free(buf);
    return res;
  }

  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  static void* wlt_mod;
  static execute_t execute;

  static const char* strSecret;
  static const char* addr;
  static const char* strSecretC;
  static const char* addrC;
};
void* OKWalletTest::wlt_mod = NULL;
execute_t OKWalletTest::execute = nullptr;

const char* OKWalletTest::strSecret = "5HxWvvfubhXpYYpS3tJkw6fq9jE9j18THftkZjHHfmFiWtmAbrj";
const char* OKWalletTest::addr = "znpLkpn6pif56eVZm9t85DWtwwEF8Rx2DHU";
const char* OKWalletTest::strSecretC = "Kwr371tjA9u2rFSMZjTNun2PXXP3WPZu2afRHTcta6KxEUdm1vEw";
const char* OKWalletTest::addrC = "znntDrEWVbxgwGJZS9qgZBter22KuXw5Cda";

TEST_F(OKWalletTest, getaddressbyprivatekey) {
  std::list<std::string> result;

  auto cmd = std::string("getaddressbyprivatekey ") + strSecret;
  execute("main", cmd, result);

  ASSERT_EQ((size_t)2, result.size());
  ASSERT_STREQ(addr, result.back().c_str());

  cmd = std::string("getaddressbyprivatekey ") + strSecretC;
  execute("main", cmd, result);

  ASSERT_EQ((size_t)2, result.size());
  ASSERT_STREQ(addrC, result.back().c_str());
}

TEST_F(OKWalletTest, createrawtransaction) {
  const auto& cmd = std::string("createrawtransaction [{\"txid\":\"b4cc287e58f87cdae59417329f710f3ecd75a4ee1d2872b7248f50977c8493f3\",\"vout\":1"
        ",\"scriptPubKey\":\"a914b10c9df5f7edf436c697f02f1efdba4cf399615187\",\"redeemScript\""
        ":\"512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052ae\"}] ") +
        std::string("{\"") + addrC + std::string("\":11}");

  std::list<std::string> result;
  execute("main", cmd, result);

  ASSERT_EQ((size_t)2, result.size());
  ASSERT_STREQ(
    "0100000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb40100000000ffffffff0100ab9041000000003c76a914ef1caef64ee1e2aff877b7e8e9713dc25b2289f288ac200206260143838b5ff52dc2eb7b4b8099d4e4c99dc3ef19794289a2cd4c10070000b400000000",
    result.back().c_str()
  );
}

TEST_F(OKWalletTest, signrawtransaction) {
  const auto& cmd = "signrawtransaction "
                    "0100000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb40100000000ffffffff0100ab9041000000003c76a914ef1caef64ee1e2aff877b7e8e9713dc25b2289f288ac200206260143838b5ff52dc2eb7b4b8099d4e4c99dc3ef19794289a2cd4c10070000b400000000 "
                    "[{\"txid\":\"b4cc287e58f87cdae59417329f710f3ecd75a4ee1d2872b7248f50977c8493f3\",\"vout\":1,\"scriptPubKey\":\"a914b10c9df5f7edf436c697f02f1efdba4cf399615187\""
                    ",\"redeemScript\":\"512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052ae\"}]"
                    " [\"KzsXybp9jX64P5ekX1KUxRQ79Jht9uzW7LorgwE65i5rWACL6LQe\",\"Kyhdf5LuKTRx4ge69ybABsiUAWjVRK4XGxAKk2FQLp2HjGMy87Z4\"]";


  std::list<std::string> result;
  execute("main", cmd, result);

  ASSERT_EQ((size_t)4, result.size());
  const auto& hex = *std::next(result.begin());
  const auto& complete = *result.rbegin();
  ASSERT_STREQ("0100000001f393847c97508f24b772281deea475cd3e0f719f321794e5da7cf8587e28ccb401000000910047304402206047ab554a70b921b883a56ad78859a61a9cd99da3c5efbdc1dd81909e3405330220610c720522a15cb7afe00210cca0ba1c7c88d01d42ff51602593c3985feef7cc0147512103debedc17b3df2badbcdd86d5feb4562b86fe182e5998abd8bcd4f122c6155b1b21027e940bb73ab8732bfdf7f9216ecefca5b94d6df834e77e108f68e66f126044c052aeffffffff0100ab9041000000003c76a914ef1caef64ee1e2aff877b7e8e9713dc25b2289f288ac200206260143838b5ff52dc2eb7b4b8099d4e4c99dc3ef19794289a2cd4c10070000b400000000",
            hex.c_str());
  ASSERT_STREQ("true", complete.c_str());
}


GTEST_API_ int main(int argc, char **argv) {
  printf("Running main() from gtest_main.cc\n");
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
