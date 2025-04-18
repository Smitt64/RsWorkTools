/*--------------------------------------------------------------
 *
 * Definition of constants used by RWMultiPrecisionInt
 *
 * (c) Copyright 1993, 1996-1997 Rogue Wave Software, Inc.
 * ALL RIGHTS RESERVED
 *
 * The software and information contained herein are proprietary to, and
 * comprise valuable trade secrets of, Rogue Wave Software, Inc., which
 * intends to preserve as trade secrets such software and information.
 * This software is furnished pursuant to a written license agreement and
 * may be used, copied, transmitted, and stored only in accordance with
 * the terms of such license and with the inclusion of the above copyright
 * notice.  This software and information or any other copies thereof may
 * not be provided or otherwise made available to any other person.
 *
 * Notwithstanding any other lease or license that may pertain to, or
 * accompany the delivery of, this computer software and information, the
 * rights of the Government regarding its use, reproduction and disclosure
 * are as set forth in Section 52.227-19 of the FARS Computer
 * Software-Restricted Rights clause.
 * 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions as set forth in subparagraph (c)(1)(ii) of the Rights in
 * Technical Data and Computer Software clause at DFARS 252.227-7013.
 * Contractor/Manufacturer is Rogue Wave Software, Inc.,
 * P.O. Box 2328, Corvallis, Oregon 97339.
 *
 * This computer software and information is distributed with "restricted
 * rights."  Use, duplication or disclosure is subject to restrictions as
 * set forth in NASA FAR SUP 18-52.227-79 (April 1985) "Commercial
 * Computer Software-Restricted Rights (April 1985)."  If the Clause at
 * 18-52.227-74 "Rights in Data General" is specified in the contract,
 * then the "Alternate III" clause applies.
 *
 *--------------------------------------------------------------*/

#ifndef __RWUTLCONST_H__
#define __RWUTLCONST_H__

#include "rw/dcmldefs.h"

#if defined(RW_MNY_NO_NEW_HEADER)
#  include <stdlib.h>
#  include <limits.h>
#else
#  include <cstdlib>
#  include <climits>
#endif
#include "rw/money/decdefs.h"

// Constants used by template metaprograms and multiple integer class.
template <class T>
class RWIntutilConstants {
public:
#if !defined(__DECCXX)
#if !defined(__hppa) || defined(HP_ANSI)
// HP-CC and DEC don't need this initialization
  RWIntutilConstants();
#endif
#endif  

// Microsoft, g++, and AIX compilers don't allow write access to constant
// data members, even when one has "cast away const" properly.  Write
// access is required to circumvent the static initialization problem.
// In SunPro 5.0 compilers, having a static const MaxDigitInt can
// cause the compiler to terminate with an assertion.

  RW_MNY_STATIC_CONST_DEL size_t HWDigitSize;
  RW_MNY_STATIC_CONST_DEL T      LowerMask;
  RW_MNY_STATIC_CONST_DEL T      UpperMask;
  RW_MNY_STATIC_CONST_DEL T      HWBorrowBit;
  RW_MNY_STATIC_CONST_DEL T      MaxDigitInt;
  RW_MNY_STATIC_CONST_DEL T      HighDigitBit;
  RW_MNY_STATIC_CONST_DEL T      LowDigitBit;

private:
  static int count;
};


#ifdef RW_MONEY_COMPILE_INSTANTIATE
#  include "rw/money/utlconst.cc"
#endif

#endif // __RWUTLCONST_H__
