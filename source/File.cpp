///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright(C) 2023 Dimo Markov <langulusteam@gmail.com>                    
///                                                                           
/// Distributed under GNU General Public License v3+                          
/// See LICENSE file, or https://www.gnu.org/licenses                         
///                                                                           
#include "File.hpp"
#include "FileSystem.hpp"


/// File constructor                                                          
///   @param producer - the file producer                                     
///   @param descriptor - instructions for configuring the GUI                
File::File(FileSystem* producer, const Descriptor& descriptor)
   : A::File {MetaOf<::File>(), descriptor}
   , ProducedFrom {producer, descriptor} {

}

/// File destruction                                                          
File::~File() {

}

/// React on environmental change                                             
void File::Refresh() {

}

Any File::ReadAs(DMeta) const {
   TODO();
}

File::StreamIn::~StreamIn() {
   TODO();
}

Offset File::StreamIn::Read(Anyness::Block&) {
   TODO();
}

File::StreamOut::~StreamOut() {
   TODO();
}

Offset File::StreamOut::Write(const Anyness::Block&) {
   TODO();
}

Ptr<A::File::StreamIn> File::NewStreamIn() {
   TODO();
}

Ptr<A::File::StreamOut> File::NewStreamOut() {
   TODO();
}