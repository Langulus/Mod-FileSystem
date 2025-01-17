///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include <Langulus/IO.hpp>

using namespace Langulus;

struct FileSystem;
struct File;
struct Folder;

LANGULUS_EXCEPTION(FileSystem);

#if 0
   #define VERBOSE_VFS(...)      Logger::Info(Self(), __VA_ARGS__)
   #define VERBOSE_VFS_TAB(...)  const auto tab = Logger::InfoTab(Self(), __VA_ARGS__)
#else
   #define VERBOSE_VFS(...)      LANGULUS(NOOP)
   #define VERBOSE_VFS_TAB(...)  LANGULUS(NOOP)
#endif

/// Include PhysFS                                                            
#include <src/physfs.h>

/// Get the last PhysFS error string                                          
LANGULUS(INLINED)
Token GetLastError() noexcept {
   const auto errorCode = PHYSFS_getLastErrorCode();
   const auto readableError = PHYSFS_getErrorByCode(errorCode);
   if (not readableError)
      return "<undefined PhysFS error code>";
   return readableError;
}