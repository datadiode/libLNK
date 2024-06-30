#include "MemoryBuffer.h"
#include "filesystemfunc.h"

namespace lnk
{

  //----------------------------------------------------------------------------------------------------------------------------------------
  // Defines, Pre-declarations & typedefs
  //----------------------------------------------------------------------------------------------------------------------------------------
  #define MINIMUM(a, b)  (((a) < (b)) ? (a) : (b))

  MemoryBuffer::MemoryBuffer(void) : 
  mBuffer(NULL),
  mSize(0)
  {
  }

  MemoryBuffer::MemoryBuffer(unsigned long iSize) : 
  mBuffer(NULL),
  mSize(0)
  {
    allocate(iSize);
  }

  MemoryBuffer::MemoryBuffer(const MemoryBuffer & iValue) : 
  mBuffer(NULL),
  mSize(0)
  {
    (*this) = iValue;
  }

  MemoryBuffer::~MemoryBuffer(void)
  {
    clear();
  }

  //----------------
  // public methods
  //----------------

  void MemoryBuffer::clear()
  {
    if (mBuffer)
    {
      delete[] mBuffer;
    }
    mBuffer = NULL;
    mSize = 0;
  }

  unsigned char * MemoryBuffer::getBuffer()
  {
    return mBuffer;
  }

  const unsigned char * MemoryBuffer::getBuffer() const
  {
    return mBuffer;
  }

  bool MemoryBuffer::allocate(unsigned long iSize)
  {
    clear();
    mBuffer = new unsigned char[iSize];
    if (mBuffer)
    {
      mSize = iSize;
      return true;
    }
    return false;
  }

  bool MemoryBuffer::reallocate(unsigned long iSize)
  {
    unsigned char * newBuffer = new unsigned char[iSize];
    if (newBuffer)
    {
      //if the current memory buffer has data
      if (mBuffer)
      {
        //copy the content of the existing buffer to newBuffer
        memcpy(newBuffer, mBuffer, MINIMUM(mSize, iSize));
      }

      clear();
      mBuffer = newBuffer;
      mSize = iSize;
      return true;
    }
    return false;
  }

  unsigned long MemoryBuffer::getSize() const
  {
    return mSize;
  }

  bool MemoryBuffer::loadFile(const char * iFilePath, unsigned long iBlobSize)
  {
    if (iBlobSize)
    {
      if (allocate(iBlobSize))
      {
        memcpy(mBuffer, iFilePath, iBlobSize);
        return true;
      }
      return false;
    }
    //get size of file
    unsigned long size = filesystem::getFileSize(iFilePath);

    FILE * f = fopen(iFilePath, "rb");
    if (f)
    {
      if (allocate(size))
      {
        fread(mBuffer, 1, size, f);

        fclose(f);
        return true;
      }
      fclose(f);
    }
    return false;
  }

  const MemoryBuffer & MemoryBuffer::operator = (const MemoryBuffer & iValue)
  {
    if (allocate(iValue.mSize))
      memcpy(mBuffer, iValue.mBuffer, mSize);
    return (*this);
  }

}; //lnk
