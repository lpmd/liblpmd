//
//
//

#ifndef __LPMD_CELLFORMAT_H__
#define __LPMD_CELLFORMAT_H__

#include <lpmd/cellreader.h>
#include <lpmd/cellwriter.h>

namespace lpmd
{

class CellFormat:public CellReader,public CellWriter
{
 public:
  CellFormat();
  virtual ~CellFormat();
};

} // lpmd

#endif
