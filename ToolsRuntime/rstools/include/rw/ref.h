#ifndef __RWREF_H__
#define __RWREF_H__

/*
 * Base class for reference counted objects.  Inherits from RWMemoryPool.
 *
 * $Id: ref.h@#/main/19  07/18/96 04:16:54  hart (TOOLS707_WIN32_971021)
 *
 * (c) Copyright 1989-1997 Rogue Wave Software, Inc.
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
 ***************************************************************************
 *
 * $Log: ref.h,v $
 * Revision 7.6  1996/07/18 04:16:54  hart
 * ObjectStore changes
 *
 * Revision 7.5  1996/05/07 22:25:06  kyle
 * Added export pragma for Mac code fragments
 *
 * Revision 7.4  1996/02/18 01:45:37  griswolf
 * Replace tabs with spaces, per Rogue Wave standard.
 *
 * Revision 7.3  1995/09/05 19:17:59  jims
 * Use new copyright macro
 *
 * Revision 7.2  1995/06/30  21:43:45  griswolf
 * Scopus #1071: make references() const.
 *
 * Revision 7.1  1994/10/16  02:54:15  josh
 * Merged 6.1 and 7.0 development trees
 *
 * Revision 6.3  1994/07/12  19:58:19  vriezen
 * Update Copyright notice
 *
 * Revision 6.2  1994/06/02  18:16:15  foote
 * Port to Symantec 7.0 on Macintosh
 *
 * Revision 6.1  1994/04/15  19:10:38  vriezen
 * Move all files to 6.1
 *
 * Revision 2.10  1993/11/18  03:28:31  myersn
 * add setRefCount(unsigned) for use by RW[CW]String.
 *
 * Revision 2.9  1993/09/13  16:29:40  keffer
 * Added forward declaration for RWMutex
 *
 * Revision 2.8  1993/09/13  04:06:12  myersn
 * make ctor argument initRef int, not unsigned, to prevent ambiguous conversion.
 *
 * Revision 2.7  1993/09/10  02:56:53  keffer
 * Switched RCS idents to avoid spurious diffs
 *
 * Revision 2.6  1993/09/01  03:37:38  myersn
 * add new constructor to allow easy static initialization by
 * depending on the linker's static init to zero to indicate one reference.
 *
 * Revision 2.5  1993/04/12  12:34:41  jims
 * Now uses RWMutex class for locking
 *
 * Revision 2.4  1993/01/29  20:26:17  myersn
 * Allow MT-safe incrementing and decrementing of reference count.
 *
 * Revision 2.2  1992/11/20  18:39:08  keffer
 * Changed type of reference count from unsigned short to unsigned.
 *
 * Revision 2.1  1992/11/01  20:34:00  keffer
 * Changed reference count from private to protected.
 *
 * Revision 2.0  1992/10/23  03:27:32  keffer
 * RCS Baseline version
 *
 * 
 *    Rev 1.1   04 Sep 1992 13:01:00   KEFFER
 * Added argument to constructor for initial reference count.
 * 
 *    Rev 1.0   11 Mar 1992 14:09:58   KEFFER
 * Initial revision.
 */

#ifndef __RWDEFS_H__
#include "rw/defs.h"
#endif

#if defined(RW_ATOMIC_INT_OPS)
#  include <windows.h>
#endif

class RWMutex;

#ifdef RW_PRAGMA_EXPORT
#pragma export on
#endif

class RWExport RWReference {
 protected:
  unsigned      refs_;          // (1 less than) number of references 
 public:
  enum RWReferenceFlag { STATIC_INIT };

  RWReference(int initRef = 0) : refs_((unsigned)initRef-1) { }
  RWReference(RWReferenceFlag) { }  // leave refs_ alone
  unsigned      references() const      {return refs_+1;}
  void          setRefCount(unsigned r) {refs_ = r-1;}
#if !defined(RW_MULTI_THREAD)
  void          addReference()          {refs_++;}
  unsigned      removeReference()       {return refs_--;}
#else
#if !defined(RW_ATOMIC_INT_OPS)
  void          addReference(RWMutex&);
  unsigned      removeReference(RWMutex&);
#else
  void          addReference() { InterlockedIncrement((long*)(&refs_)); }
  unsigned      removeReference()
  {
    unsigned result = InterlockedExchangeAdd((long*)(&refs_), -1);
    return result;
  }
#endif
#endif

  RW_TYPESPEC  /* This macro usually expands to nothing */
};

#ifdef RW_PRAGMA_EXPORT
#pragma export off
#endif

#endif  /* __RWREF_H__ */
