/*
    Copyright (c) 2010, Peter Johansson <zeronightfall@gmx.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
    MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

/*!
 * Specifies the interface for each service/daemon that is going to be
 * started during system initialization or stopped during system shutdown.
 * The interface also includes information that can be used to calculate the
 * dependencies.
 */
typedef struct service_t {
    /*! Contains a function pointer to a function which returns the name of
     *  the service/daemon. This is used for identify a service/daemon. */
    const char* (*get_name)(void);
    /*! Contains a function pointer to a function which returns a common name
     *  for a service/daemon. Used when there are several services/daemons that
     *  provides the same functionality. */
    const char* (*provides)(void);
    /*! Contains a function pointer to a function which returns all the
     *  dependencies that needs to be started before the current service/daemon
     *  has started.
     *  \note Currently there isn't any decision if the dependency should be
     *        mandatory or optional. This might be divided in future into
     *        two dependency lists where one is optional and the other is
     *        mandatory. */
    const char** (*get_dependency)(void);
    /*! Contains a function pointer to a function which is used for starting
     *  up the service or the daemon during system initialization. */
    int (*initialization)(void);
    /*! Contains a function pointer to a function which is used for stopping
     *  the service or the daemon during system shutdown. */
    int (*shutdown)(void);
} service_t;
