#include "file_layer/file_layer.h"

#include "base/log.h"
#include "guest.h"

namespace file_layer {

void FileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8258C938 File::File");
  guest::StoreU32(base, file + 8, 0);
  guest::StoreU32(base, file + 0, kFileVtable);
  guest::StoreU32(base, file + 4, 0xFFFFFFFFu);
  guest::StoreU32(base, file + 12, 0);
}

void FileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_8258C5F8 File::~File");
  guest::StoreU32(base, file, kStreamVtable);
}

void AsyncFileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593AC0 AsyncFile::AsyncFile");
  FileConstruct(base, file);
  guest::StoreU32(base, file, kAsyncFileVtable);
}

void AsyncFileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_82593B00 AsyncFile::~AsyncFile");
  guest::StoreU32(base, file, kAsyncFileVtable);
  FileDestruct(base, file);
}

void GameFileConstruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A00 GameFile::GameFile");
  AsyncFileConstruct(base, file);
  guest::StoreU32(base, file, kGameFileVtable);
}

void GameFileDestruct(uint8_t* base, uint32_t file) {
  ITC_NATIVE_RUNNING("sub_820A6A40 GameFile::~GameFile");
  guest::StoreU32(base, file, kGameFileVtable);
  AsyncFileDestruct(base, file);
}

void PathConstruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C28 Path::Path");
  guest::StoreU8(base, path + 4, 0);
  guest::StoreU32(base, path, kPathVtable);
}

void PathDestruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_82593C48 Path::~Path");
  guest::StoreU32(base, path, kPathVtable);
}

void GamePathConstruct(uint8_t* base, uint32_t path) {
  ITC_NATIVE_RUNNING("sub_820A6BE0 GamePath::GamePath");
  guest::StoreU32(base, path, kGamePathVtable);
}

}  // namespace file_layer
