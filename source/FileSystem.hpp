///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "File.hpp"
#include "Folder.hpp"
#include <Flow/Verbs/Create.hpp>


///                                                                           
///   File system                                                             
///                                                                           
struct FileSystem final : A::FileSystem {
   LANGULUS(ABSTRACT) false;
   LANGULUS_BASES(A::FileSystem);
   LANGULUS_VERBS(Verbs::Create);

private:
   // List of interfaced files                                          
   TFactory<File> mFiles;
   // List of interfaced folders                                        
   TFactory<Folder> mFolders;

public:
   FileSystem(Runtime*, const Descriptor&);

   void Update(Time);
   void Create(Verb&);

   NOD() const A::File*    GetFile(const Path&) const;
   NOD() const A::Folder*  GetFolder(const Path&) const;
};

