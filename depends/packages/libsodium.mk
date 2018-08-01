package=libsodium
$(package)_version=1.0.11
$(package)_download_path=https://download.libsodium.org/libsodium/releases/old/
$(package)_file_name=libsodium-1.0.11.tar.gz
$(package)_sha256_hash=a14549db3c49f6ae2170cbbf4664bd48ace50681045e8dbea7c8d9fb96f9c765
$(package)_dependencies=
$(package)_config_opts=

define $(package)_preprocess_cmds
  cd $($(package)_build_subdir); ./autogen.sh
endef

CCX_VAR=
BUILD_OS := $(shell uname)
ifeq ($(BUILD_OS),Darwin)
	CCX_VAR=CC=gcc CCAS=gcc CPP="gcc -E"
endif

define $(package)_config_cmds
  $($(package)_autoconf) --enable-static --disable-shared --with-pic=yes $(CCX_VAR)
endef

define $(package)_build_cmds
  $(MAKE)
endef

define $(package)_stage_cmds
  $(MAKE) DESTDIR=$($(package)_staging_dir) install
endef
