///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "File.hpp"
#include "Folder.hpp"
#include <Langulus/Factory.hpp>
#include <Langulus/Verbs/Create.hpp>


///                                                                           
///   File system                                                             
///                                                                           
struct FileSystem final : Things::FileSystem {
   using CTTI_Abstract  = No;
   using CTTI_Bases     = Things::FileSystem;
   using CTTI_Abilities = Types<Verbs::Create, Verbs::Select>;

private:
   // List of interfaced files                                          
   TFactoryUnique<File> mFiles;
   // Files indexed by a lowercase relative path                        
   TMapUnsorted<Path, Ref<Things::File>> mFileMap;

   // List of interfaced folders                                        
   TFactoryUnique<Folder> mFolders;
   // Folders indexed by a lowercase relative path                      
   TMapUnsorted<Path, Ref<Things::Folder>> mFolderMap;

public:
    FileSystem(Things::Runtime*, Many const&);
   ~FileSystem();

   bool Update(Time);
   void Create(Verb&);
   void Select(Verb&);
   void Teardown();

   auto GetFile  (Path const&) -> Ref<Things::File>;
   auto GetFolder(Path const&) -> Ref<Things::Folder>;
};

