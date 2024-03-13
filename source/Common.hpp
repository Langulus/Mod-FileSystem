///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include <Langulus/IO.hpp>

using namespace Langulus;

struct FileSystem;
struct File;
struct Folder;

LANGULUS_EXCEPTION(FileSystem);

#define VERBOSE_VFS(...)      //Logger::Verbose(Self(), __VA_ARGS__)
#define VERBOSE_VFS_TAB(...)  //const auto tab = Logger::Verbose(Self(), __VA_ARGS__, Logger::Tabs {})

/// Include PhysFS                                                            
#include <src/physfs.h>

/// Get the last PhysFS error string                                          
NOD() LANGULUS(INLINED)
Token GetLastError() noexcept {
   const auto errorCode = PHYSFS_getLastErrorCode();
   const auto readableError = PHYSFS_getErrorByCode(errorCode);
   if (not readableError)
      return "<undefined PhysFS error code>";
   return readableError;
}