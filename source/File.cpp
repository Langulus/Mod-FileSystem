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
   // Notice provided descriptor isn't forwarded to factory, because    
   // we always substitute it with a lowercase file path, so that files 
   // are unique based only on that                                     
   , ProducedFrom {producer, {}} {
   VERBOSE_VFS("Initializing...");

   // Parse the descriptor for a filename                               
   descriptor.ForEachDeep(
      [&](const Text& text) {
         mFilePath = text;
      },
      [&](const Trait& trait) {
         if (trait.TraitIs<Traits::Name, Traits::File>())
            mFilePath = trait.AsCast<Text>();
      }
   );
   LANGULUS_ASSERT(!mFilePath.IsEmpty(), FileSystem,
      "Can't interface empty file path");

   mFilePath = mFilePath.Terminate();
   mParentDirectory = mFilePath.GetDirectory();
   mFileName = mFilePath.GetFilename();
   mFileExtension = mFilePath.GetExtension();

   // Check if file exists, and retrieve its info                       
   if (0 != PHYSFS_stat(mFilePath.GetRaw(), &mFileInfo)) {
      LANGULUS_ASSERT(
         mFileInfo.filetype == PHYSFS_FILETYPE_REGULAR, FileSystem,
         "Path `", mFilePath, "` doesn't point to a regular file"
      );

      mExists = true;
      mByteCount = mFileInfo.filesize;
   }

   // Check the file format, if available                               
   auto& candidates = RTTI::Database.ResolveFileExtension(mFileExtension);
   switch(candidates.size()) {
   case 0:
      Logger::Warning(Self(),
         "Unknown file extension: ", mFilePath);
      break;
   case 1:
      mFormat = dynamic_cast<DMeta>(*candidates.begin());
      if (!mFormat) {
         Logger::Warning(Self(),
            "File extension isn't associated with data: ", mFilePath);
      }
      break;
   default:
      Logger::Warning(Self(),
         "Ambiguous file extension for `", mFilePath,
         "`, could be any of the following: "
      );
      for (auto ext : candidates)
         Logger::Warning("  * ", ext);
   }

   VERBOSE_VFS("Initialized");
}

/// File destruction                                                          
File::~File() {
   Close();
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

/// Read bytes into a preallocated block                                      
///   @param output - [out] the read bytes go here                            
///   @return the number of read bytes                                        
Offset File::StreamIn::Read(Anyness::Block& output) {
   const auto count = PHYSFS_uint64(output.GetSize());
   const auto result = PHYSFS_readBytes(mFile, output.GetBytes(), count);
   VERBOSE_VFS("Reads ", ByteCount(result), " from ", mFilename);

   LANGULUS_ASSERT(-1 != result, FileSystem,
      "Complete failure in PHYSFS_readBytes: ", GetLastPhysFSErrorLiteral());
   LANGULUS_ASSERT(PHYSFS_uint64(result) == count, FileSystem,
      "Error in PHYSFS_readBytes: ", GetLastPhysFSErrorLiteral());

   const auto r = static_cast<Offset>(result);
   mProgress += r;
   return r;
}

File::StreamOut::~StreamOut() {
   TODO();
}

/// Read bytes from a preallocated block                                      
///   @param input - the written bytes come from here                         
///   @return the number of written bytes                                     
Offset File::StreamOut::Write(const Anyness::Block&) {
   const auto count = PHYSFS_uint64(input.GetSize());
   const auto result = PHYSFS_writeBytes(mFile, input.GetBytes(), count);
   VERBOSE_VFS("Writes ", ByteCount(result), " to ", mFilename);

   LANGULUS_ASSERT(PHYSFS_uint64(result) == count, FileSystem,
      "Error in PHYSFS_writeBytes: ", GetLastPhysFSErrorLiteral());

   const auto r = static_cast<Offset>(result);
   mProgress += r;
   return r;
}

Ptr<A::File::StreamIn> File::NewStreamIn() {
   TODO();
}

Ptr<A::File::StreamOut> File::NewStreamOut() {
   TODO();
}

/// Read or write to the file                                                 
///   @param verb - the read/write verb                                       
void File::Read(Verb& verb) {
   if (verb.GetMass() > 0) {
      // We're reading                                                  
      VERBOSE_VFS("Reading at " << index);
      TODO();
      verb.Done();
   }

   if (verb.GetMass() < 0) {
      // We're writing                                                  
      VERBOSE_VFS("Writing at " << index);
      TODO();
      verb.Done();
   }
}

/// Open a file, locking it                                                   
///   @param mode - the intent for the file                                   
///   @return true on success                                                 
Ptr<A::File::Stream> File::Open(const FState& mode) {
   if (mFile && mode != mState) {
      throw Except::FileIO(pcLogSelfError
         << "File `" << mRelativeFilename
         << "` is already opened with another intent: " << mState);
   }

   mState = mode;

   if (mState & FState::Read) {
      // Open file for reading														
      mFile = PHYSFS_openRead(mRelativeFilename.GetRaw());
      if (!mFile) {
         throw Except::FileIO(pcLogSelfError
            << "Can't open `" << mRelativeFilename << "` for reading");
      }
   }
   else if (mState & FState::Write) {
      // Open file anew for writing													
      if (mExists && mFileInfo.readonly) {
         throw Except::FileIO(pcLogSelfError
            << "Can't open read-only `" << mRelativeFilename << "` for writing");
      }

      mFile = PHYSFS_openWrite(mRelativeFilename.GetRaw());
      if (!mFile) {
         throw Except::FileIO(pcLogSelfError
            << "Can't open `" << mRelativeFilename << "` for writing");
      }
   }
   else if (mode & FState::Append) {
      // Open file and write to the end											
      if (mExists && mFileInfo.readonly) {
         throw Except::FileIO(pcLogSelfError
            << "Can't open read-only `" << mRelativeFilename << "` for appending");
      }

      mFile = PHYSFS_openAppend(mRelativeFilename.GetRaw());
      if (!mFile) {
         throw Except::FileIO(pcLogSelfError
            << "Can't open `" << mRelativeFilename << "` for appending");
      }
   }
   else {
      throw Except::FileIO(pcLogSelfError
         << "File `" << mRelativeFilename << "` has bad mode: " << mState);
   }

   return {this};
}

/// Close the file, if it was opened                                          
void File::Close() {
   if (!mFile)
      return;

   LANGULUS_ASSERT(PHYSFS_close(mFile), FileSystem,
      "Error in PHYSFS_close: ", GetLastPhysFSErrorLiteral());
   mFile = nullptr;
}