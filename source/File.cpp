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
      mIsReadOnly = mFileInfo.readonly;
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

/// Read a file and deserialize it as the required type                       
Any File::ReadAs(DMeta) const {
   TODO();
}

/// Create a new file reader                                                  
Ptr<A::File::Reader> File::NewReader() {
   TODO();
}

/// Create a new file writer                                                  
Ptr<A::File::Writer> File::NewWriter(bool append) {
   TODO();
}

/// Rewrite the file, by serializing the verb's arguments                     
///   @param verb - the associate verb                                        
void File::Associate(Verb& verb) {
   if (verb.GetMass() <= 0)
      return;

   TODO();
}

/// Append to the file, by serializing the verb's arguments                   
///   @param verb - the catenate verb                                         
void File::Catenate(Verb& verb) {
   if (verb.GetMass() <= 0)
      return;

   TODO();
}

/// Seek a position inside the file, like the front/back/specific offset      
///   @param verb - the select verb                                           
void File::Select(Verb& verb) {
   if (verb.GetMass() <= 0)
      return;

   TODO();
}

/// Attempt to interpret the file contents at the current position as some    
/// data type                                                                 
///   @param verb - the interpret verb                                        
void File::Interpret(Verb& verb) {
   TODO();
}



///                                                                           
///   File reader implementation                                              
///                                                                           

/// File reader constructor                                                   
///   @param file - the file interface                                        
File::Reader::Reader(File* file)
   : A::File::Reader {file} {
   // Open file for reading                                             
   mHandle = PHYSFS_openRead(mFile->GetFilePath().GetRaw());
   LANGULUS_ASSERT(mHandle, FileSystem,
      "Can't open `", mFile->GetFilePath(), "` for reading");
}

/// File reader destructor                                                    
File::Reader::~Reader() {
   if (!mHandle)
      return;

   // Close the file handle                                             
   LANGULUS_ASSERT(PHYSFS_close(mHandle), FileSystem,
      "Error in PHYSFS_close: ", GetLastError());
}

/// Read bytes into a preallocated block                                      
///   @param output - [out] the read bytes go here                            
///   @return the number of read bytes                                        
Offset File::Reader::Read(Block& output) {
   const auto count = PHYSFS_uint64(output.GetByteSize());
   const auto result = PHYSFS_readBytes(mHandle, output.GetRaw(), count);
   VERBOSE_VFS("Reads ", ByteCount {result}, " from ", mFile->GetFilePath());

   LANGULUS_ASSERT(-1 != result, FileSystem,
      "Complete failure in PHYSFS_readBytes: ", GetLastError());
   LANGULUS_ASSERT(PHYSFS_uint64(result) == count, FileSystem,
      "Error in PHYSFS_readBytes: ", GetLastError());

   const auto r = static_cast<Offset>(result);
   mProgress += r;
   return r;
}

Debug File::Reader::Self() const {
   return mFile ? mFile->Self() : "<invalid file reader>";
}



///                                                                           
///   File writer implementation                                              
///                                                                           

/// File writer constructor                                                   
///   @param file - the file interface                                        
///   @param append - false if you want to delete and create the file anew    
File::Writer::Writer(File* file, bool append)
   : A::File::Writer {file, append} {
   // Check if file is read-only                                        
   LANGULUS_ASSERT(
      !mFile->Exists() || !mFile->IsReadOnly(), FileSystem,
      "Can't open read-only `", mFile->GetFilePath(), "` for writing/appending"
   );

   if (append) {
      // Open file for appending                                        
      mHandle = PHYSFS_openAppend(mFile->GetFilePath().GetRaw());
      LANGULUS_ASSERT(mHandle, FileSystem,
         "Can't open `", mFile->GetFilePath(), "` for appending");
   }
   else {
      // Open file anew for writing                                     
      mHandle = PHYSFS_openWrite(mFile->GetFilePath().GetRaw());
      LANGULUS_ASSERT(mHandle, FileSystem,
         "Can't open `", mFile->GetFilePath(), "` for writing");
   }
}

/// File writer destructor                                                    
File::Writer::~Writer() {
   if (!mHandle)
      return;

   // Close the file handle                                             
   LANGULUS_ASSERT(PHYSFS_close(mHandle), FileSystem,
      "Error in PHYSFS_close: ", GetLastError());
}

/// Write bytes to a preallocated block                                       
///   @param input - the written bytes come from here                         
///   @return the number of written bytes                                     
Offset File::Writer::Write(const Block& input) {
   const auto count = PHYSFS_uint64(input.GetByteSize());
   const auto result = PHYSFS_writeBytes(mHandle, input.GetRaw(), count);
   VERBOSE_VFS("Writes ", ByteCount(result), " to ", mFile->GetFilePath());

   LANGULUS_ASSERT(PHYSFS_uint64(result) == count, FileSystem,
      "Error in PHYSFS_writeBytes: ", GetLastError());

   const auto r = static_cast<Offset>(result);
   mProgress += r;
   return r;
}

Debug File::Writer::Self() const {
   return mFile ? mFile->Self() : "<invalid file writer>";
}