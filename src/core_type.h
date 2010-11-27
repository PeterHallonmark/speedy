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
    /*! The name of the service/daemon. This is used for identify a
     *  service/daemon. */
    char* name;
    /*! A common name for a service/daemon. Used when there are several
     *  services/daemons that provides the same functionality. */
    char* provides;
    /*! Contains the dependencies that needs to be started before the current
     *  service/daemon can start.
     *  \note Currently there isn't any decision if the dependency should be
     *        mandatory or optional. This might be divided in future into
     *        two dependency lists where one is optional and the other is
     *        mandatory. */
    char** dependency;
    /*! Contains a function pointer to a function which is used for
        executing a certain action. */
    int (*action)(void);
} service_t;
