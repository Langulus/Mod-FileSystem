///                                                                           
/// Langulus::Module::FileSystem                                              
/// Copyright (c) 2016 Dimo Markov <team@langulus.com>                        
/// Part of the Langulus framework, see https://langulus.com                  
///                                                                           
/// SPDX-License-Identifier: GPL-3.0-or-later                                 
///                                                                           
#include "File.hpp"
#include "FileSystem.hpp"


/// File constructor                                                          
///   @param producer - the file producer                                     
///   @param descriptor - instructions for configuring the GUI                
File::File(FileSystem* producer, const Many& descriptor)
   : Resolvable   {this}
   , ProducedFrom {producer, descriptor} {
   VERBOSE_VFS("Initializing...");

   // Get a path from the descriptor                                    
   if (not descriptor.ExtractTrait<Traits::Name, Traits::Path>(mFilePath))
      descriptor.ExtractDataAs(mFilePath);
   LANGULUS_ASSERT(mFilePath, FileSystem,
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
      mByteCount = static_cast<Offset>(mFileInfo.filesize);
      mIsReadOnly = mFileInfo.readonly;
      VERBOSE_VFS("Interfaces existing file: ", mFilePath);
   }
   else {
      VERBOSE_VFS("Interfaces non-existing file: ", mFilePath);
   }

#if LANGULUS_FEATURE(MANAGED_REFLECTION)
   // Check the file format, if available                               
   auto& candidates = RTTI::ResolveFileExtension(mFileExtension);
   switch(candidates.size()) {
   case 0:
      Logger::Warning(Self(),
         "Unknown file extension: `", mFilePath, '`');
      break;
   case 1:
      mFormat = *candidates.begin();
      VERBOSE_VFS("File format detected: ", mFormat);
      if (not mFormat) {
         Logger::Warning(Self(),
            "File extension isn't associated with data: `", mFilePath, '`');
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
#endif

   Couple(descriptor);
   VERBOSE_VFS("Initialized");
}

/// React on environmental change                                             
void File::Detach() {
   mFilePath.Reset();
   ProducedFrom<FileSystem>::Detach();
}

/// Read a file and deserialize it as the required type                       
Many File::ReadAs(DMeta) const {
   TODO();
   return {};
}

/// Create a new file reader                                                  
///   @return a pointer to the file reader                                    
Ref<A::File::Reader> File::NewReader() const {
   Reader newReader {const_cast<File*>(this)};
   return &mReaders.Emplace(IndexBack, Abandon(newReader));
}

/// Create a new file writer                                                  
///   @return a pointer to the file writer                                    
Ref<A::File::Writer> File::NewWriter(bool append) const {
   LANGULUS_ASSERT(not mWriter, FileSystem,
      "File `", mFilePath, "` is already opened for writing");
   Writer newWriter {const_cast<File*>(this), append};
   return &mWriter.emplace(Abandon(newWriter));
}

/// Get a file interface with filename, relative to this file                 
///   @param filename - path relative to this file's path                     
///   @return the file interface                                              
Ref<A::File> File::RelativeFile(const Path& filename) const {
   return GetProducer()->GetFile(Path {mParentDirectory} / filename);
}

/// Get a subfolder interface with filename, relative to this file            
///   @param dirname - path relative to this file's path                      
///   @return the folder interface                                            
Ref<A::Folder> File::RelativeFolder(const Path& dirname) const {
   return GetProducer()->GetFolder(Path {mParentDirectory} / dirname);
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
void File::Interpret(Verb&) {
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

/// Explicit file abandon-construction                                        
///   @param file - the file interface                                        
File::Reader::Reader(Abandoned<Reader>&& other)
   : A::File::Reader {*other}
   , mHandle {other->mHandle} {
   other->mHandle = nullptr;
}

/// File reader destructor                                                    
File::Reader::~Reader() {
   if (not mHandle)
      return;

   // Close the file handle                                             
   LANGULUS_ASSERT(PHYSFS_close(mHandle.Get()), FileSystem,
      "Error in PHYSFS_close: ", GetLastError());
}

/// Read bytes into a preallocated block                                      
///   @attention output might not be entirely filled, check return value      
///   @param output - [out] the read bytes go here                            
///   @return the true number of read bytes                                   
Offset File::Reader::Read(Many& output) {
   const auto count = PHYSFS_uint64(output.GetBytesize());
   const auto result = PHYSFS_readBytes(mHandle.Get(), output.GetRaw(), count);
   const auto r = static_cast<Offset>(result);
   VERBOSE_VFS("Reads ", Size {r}, " from `", mFile->GetFilePath(), '`');

   LANGULUS_ASSERT(-1 != result, FileSystem,
      "Complete failure in PHYSFS_readBytes: ", GetLastError());
   //LANGULUS_ASSERT(r == count, FileSystem,
   //   "Error in PHYSFS_readBytes: ", GetLastError());

   mProgress += r;
   return r;
}

Text File::Reader::Self() const {
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
      not mFile->Exists() or not mFile->IsReadOnly(), FileSystem,
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

/// Writer abandon-construction                                               
///   @param other - the writer                                               
File::Writer::Writer(Abandoned<Writer>&& other)
   : A::File::Writer {*other}
   , mHandle {other->mHandle} {
   other->mHandle = nullptr;
}

/// File writer destructor                                                    
File::Writer::~Writer() {
   if (not mHandle)
      return;

   // Close the file handle                                             
   LANGULUS_ASSERT(PHYSFS_close(mHandle.Get()), FileSystem,
      "Error in PHYSFS_close: ", GetLastError());
}

/// Write bytes to a preallocated block                                       
///   @param input - the written bytes come from here                         
///   @return the number of written bytes                                     
Offset File::Writer::Write(const Many& input) {
   const auto count = PHYSFS_uint64(input.GetBytesize());
   const auto result = static_cast<Offset>(
      PHYSFS_writeBytes(mHandle.Get(), input.GetRaw(), count));

   VERBOSE_VFS("Writes ", result, " to `", mFile->GetFilePath(), '`');
   LANGULUS_ASSERT(PHYSFS_uint64(result) == count, FileSystem,
      "Error in PHYSFS_writeBytes: ", GetLastError());

   const auto r = static_cast<Offset>(result);
   mProgress += r;
   return r;
}

Text File::Writer::Self() const {
   return mFile ? mFile->Self() : "<invalid file writer>";
}
