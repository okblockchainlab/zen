set(ZEN_SOURCE
${PROJECT_SOURCE_DIR}/../src/test/test_bitcoin.cpp
)

set(WALLET_LIB_SOURCE
  wallet.cpp
  app_wrapper.cpp
  com_okcoin_vault_jni_zen_Zenj.cpp
)

add_definitions(-DHAVE_CONFIG_H)

include_directories(${PROJECT_SOURCE_DIR}/../src)
include_directories(${PROJECT_SOURCE_DIR}/../src/config)
#include_directories(${PROJECT_SOURCE_DIR}/../src/secp256k1)
include_directories(${PROJECT_SOURCE_DIR}/../src/leveldb/include)
include_directories(${PROJECT_SOURCE_DIR}/../src/univalue/include)

include_directories($ENV{JAVA_HOME}/include)
if(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
  include_directories($ENV{JAVA_HOME}/include/darwin)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  include_directories($ENV{JAVA_HOME}/include/linux)
endif()

add_library(${OKLIBRARY_NAME} SHARED ${ZEN_SOURCE} ${WALLET_LIB_SOURCE})
target_link_libraries(${OKLIBRARY_NAME}
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_server.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_wallet.a
  ${PROJECT_SOURCE_DIR}/../src/leveldb/libleveldb.a
  ${PROJECT_SOURCE_DIR}/../src/leveldb/libmemenv.a
  ${PROJECT_SOURCE_DIR}/../src/crypto/libbitcoin_crypto.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_common.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_cli.a
  ${PROJECT_SOURCE_DIR}/../src/libzcash.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_util.a
  ${PROJECT_SOURCE_DIR}/../src/secp256k1/.libs/libsecp256k1.a
  ${PROJECT_SOURCE_DIR}/../src/libbitcoin_zmq.a
  ${PROJECT_SOURCE_DIR}/../src/univalue/.libs/libunivalue.a
  ${PROJECT_SOURCE_DIR}/../src/.libs/libzcashconsensus.a
  ${DEPENDS_LIB}/lib/libsnark.a
  ${DEPENDS_LIB}/lib/libboost_filesystem-mt.a
  ${DEPENDS_LIB}/lib/libboost_thread-mt.a
  ${DEPENDS_LIB}/lib/libboost_chrono-mt.a
  ${DEPENDS_LIB}/lib/libboost_system-mt.a
  ${DEPENDS_LIB}/lib/libssl.a
  ${DEPENDS_LIB}/lib/libcrypto.a
  ${DEPENDS_LIB}/lib/libminiupnpc.a
  ${DEPENDS_LIB}/lib/libdb_cxx.a
  ${DEPENDS_LIB}/lib/libboost_program_options-mt.a
  ${DEPENDS_LIB}/lib/libboost_unit_test_framework-mt.a
  ${DEPENDS_LIB}/lib/libgmp.a
  ${DEPENDS_LIB}/lib/libsodium.a
  ${DEPENDS_LIB}/lib/librustzcash.a
)

################################################################################

include_directories(${PROJECT_SOURCE_DIR}/src)
add_test(test_okwallet test_okwallet)
add_executable(test_okwallet ${PROJECT_SOURCE_DIR}/test/src/test_okwallet.cc)
target_link_libraries(test_okwallet
  ${DEPENDS_LIB}/lib/libgtest.a
)
