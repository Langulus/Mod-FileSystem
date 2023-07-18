///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#pragma once
#include "Common.hpp"


///                                                                           
///   A file                                                                  
///                                                                           
struct File final : A::File, ProducedFrom<FileSystem> {
   LANGULUS(ABSTRACT) false;
   LANGULUS(PRODUCER) FileSystem;
   LANGULUS_BASES(A::File);

private:
   Path mFilePath;

public:
   File(FileSystem*, const Descriptor&);
   ~File();

   void Refresh();

   NOD() Any ReadAs(DMeta) const;

   struct StreamIn final : A::File::StreamIn {
      ~StreamIn();
      Offset Read(Anyness::Block&);
   };

   struct StreamOut final : A::File::StreamOut {
      ~StreamOut();
      Offset Write(const Anyness::Block&);
   };

   NOD() Ptr<A::File::StreamIn>  NewStreamIn();
   NOD() Ptr<A::File::StreamOut> NewStreamOut();
};