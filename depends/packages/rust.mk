package=rust
$(package)_version=1.16.0
$(package)_download_path=https://static.rust-lang.org/dist

BUILD_OS := $(shell uname)
ifeq ($(BUILD_OS),Darwin)
  $(package)_file_name=rust-$($(package)_version)-x86_64-apple-darwin.tar.gz
  $(package)_sha256_hash=2d08259ee038d3a2c77a93f1a31fc59e7a1d6d1bbfcba3dba3c8213b2e5d1926
else
  $(package)_file_name=rust-$($(package)_version)-x86_64-unknown-linux-gnu.tar.gz
  $(package)_sha256_hash=48621912c242753ba37cad5145df375eeba41c81079df46f93ffb4896542e8fd
endif


define $(package)_stage_cmds
  ./install.sh --destdir=$($(package)_staging_dir) --prefix=$(host_prefix)/native --disable-ldconfig
endef
