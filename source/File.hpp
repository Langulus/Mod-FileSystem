///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"
#include <Flow/Factory.hpp>
#include <Flow/Verbs/Associate.hpp>
#include <Flow/Verbs/Catenate.hpp>
#include <Flow/Verbs/Select.hpp>
#include <Flow/Verbs/Interpret.hpp>
#include <optional>


///                                                                           
///   A file                                                                  
///                                                                           
struct File final : A::File, Flow::ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::File);
   LANGULUS_VERBS(
      Verbs::Associate,
      Verbs::Catenate,
      Verbs::Select,
      Verbs::Interpret
   );

   /// File reader stream                                                     
   struct Reader final : A::File::Reader {
   private:
      // The low-level file interface                                   
      Own<PHYSFS_File*> mHandle;

      Text Self() const;

   public:
      Reader(File*);
      Reader(Abandoned<Reader>&&);
      ~Reader();

      Offset Read(Many&);
   };

   /// File writer stream                                                     
   struct Writer final : A::File::Writer {
   private:
      // The low-level file interface                                   
      Own<PHYSFS_File*> mHandle;

      Text Self() const;

   public:
      Writer(File*, bool append);
      Writer(Abandoned<Writer>&&);
      ~Writer();

      Offset Write(const Many&);
   };

private:
   // Parent directory substring, mapped onto A::File::mFilePath        
   Token mParentDirectory;
   // File name after all directories, mapped onto A::File::mFilePath   
   Token mFileName;
   // The extension of the filename, mapped onto A::File::mFilePath     
   Token mFileExtension;

   // Information about the file, if file exists                        
   PHYSFS_Stat mFileInfo {};

   // Active file reader streams                                        
   mutable TMany<Reader> mReaders;
   // Active file writer stream (there can be only one)                 
   mutable std::optional<Writer> mWriter;

public:
   File(FileSystem*, const Neat&);

   void Detach();
   void Refresh() {}

   void Associate(Verb&);
   void Catenate(Verb&);
   void Select(Verb&);
   void Interpret(Verb&);

   NOD() Many ReadAs(DMeta) const;

   NOD() Ref<A::File::Reader> NewReader() const;
   NOD() Ref<A::File::Writer> NewWriter(bool append) const;

   NOD() Ref<A::File>   RelativeFile(const Path&) const;
   NOD() Ref<A::Folder> RelativeFolder(const Path&) const;
};