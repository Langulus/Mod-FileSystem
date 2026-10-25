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
   mutable TFactoryUnique<File> mFiles;
   // Files indexed by a lowercase relative path                        
   mutable TMapUnsorted<Path, Ref<Things::File>> mFileMap;

   // List of interfaced folders                                        
   mutable TFactoryUnique<Folder> mFolders;
   // Folders indexed by a lowercase relative path                      
   mutable TMapUnsorted<Path, Ref<Things::Folder>> mFolderMap;

public:
    FileSystem(Runtime*, Many const&);
   ~FileSystem();

   void Create(Verb&);
   void Select(Verb&);

   bool Update(Time) override;
   void Teardown() override;
   auto GetFile  (Path const&) const -> Ref<Things::File> override;
   auto GetFolder(Path const&) const -> Ref<Things::Folder> override;
};

