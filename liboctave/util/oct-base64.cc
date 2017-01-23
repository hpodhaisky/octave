/*

Copyright (C) 2012-2016 John W. Eaton

This file is part of Octave.

Octave is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3 of the License, or
(at your option) any later version.

Octave is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Octave; see the file COPYING.  If not, see
<http://www.gnu.org/licenses/>.

*/

#if defined (HAVE_CONFIG_H)
#  include "config.h"
#endif

#include <algorithm>

#include "Array.h"
#include "base64-wrappers.h"
#include "oct-base64.h"

bool
octave_base64_encode (const char *inc, const size_t inlen, char **out)
{
  bool ret = false;

  size_t outlen = octave_base64_encode_alloc_wrapper (inc, inlen, out);

  if (! out)
    {
      if (outlen == 0 && inlen != 0)
        (*current_liboctave_error_handler)
          ("base64_encode: input array too large");
      else
        (*current_liboctave_error_handler)
          ("base64_encode: memory allocation error");
    }
  else
    ret = true;

  return ret;
}

Array<double>
octave_base64_decode (const std::string& str)
{
  Array<double> retval;

  char *out;
  size_t outlen;

  bool ok = octave_base64_decode_alloc_wrapper (str.data (), str.length (),
                                                &out, &outlen);

  if (! ok)
    (*current_liboctave_error_handler)
      ("base64_decode: input was not valid base64");
  if (! out)
    (*current_liboctave_error_handler)
      ("base64_decode: memory allocation error");

  if ((outlen % (sizeof (double) / sizeof (char))) != 0)
    {
      ::free (out);
      (*current_liboctave_error_handler)
        ("base64_decode: incorrect input size");
    }
  else
    {
      octave_idx_type len = (outlen * sizeof (char)) / sizeof (double);
      retval.resize (dim_vector (1, len));
      double *dout = reinterpret_cast<double*> (out);
      std::copy (dout, dout + len, retval.fortran_vec ());
      ::free (out);
    }

  return retval;
}
