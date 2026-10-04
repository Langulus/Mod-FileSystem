///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#pragma once
#include "Export.hpp"
#include <Langulus/Producible.hpp>
#include <Langulus/Verbs/Associate.hpp>
#include <Langulus/Verbs/Catenate.hpp>
#include <Langulus/Verbs/Select.hpp>
#include <Langulus/Verbs/Interpret.hpp>
#include <optional>


///                                                                           
///   A file                                                                  
///                                                                           
struct File final : Langulus::File, Flow::ProducedFrom<FileSystem> {
   using CTTI_Abstract = No;
   using CTTI_Producer = FileSystem;
   LANGULUS_BASES(A::File);
   LANGULUS_VERBS(
      Verbs::Associate,
      Verbs::Catenate,
      Verbs::Select,
      Verbs::Interpret
   );


   ///                                                                        
   /// File reader stream                                                     
   struct Reader final : Langulus::File::Reader {
   private:
      Text Self() const;

   public:
      Reader(File*);

      size_t Read(Many&);
   };


   ///                                                                        
   /// File writer stream                                                     
   struct Writer final : Langulus::File::Writer {
   private:
      Text Self() const;

   public:
      Writer(File*, bool append);

      size_t Write(Many const&);
   };

protected:
   friend struct Reader;
   friend struct Writer;

   // Information about the file, if file exists                        
   PHYSFS_Stat mFileInfo {};
   // Opened file handle                                                
   mutable Own<PHYSFS_File*> mHandle;

public:
   File(FileSystem*, Many const&);
  ~File();

   void Refresh() {}
   void Teardown();

   void Associate(Verb&);
   void Catenate(Verb&);
   void Select(Verb&);
   void Interpret(Verb&);

   Many ReadAs(DMeta) const;

   auto NewReader()                 const -> Ref<Langulus::File::Reader>;
   auto NewWriter(bool append)      const -> Ref<Langulus::File::Writer>;

   auto RelativeFile(const Path&)   const -> Ref<Langulus::File>;
   auto RelativeFolder(const Path&) const -> Ref<Langulus::Folder>;
};