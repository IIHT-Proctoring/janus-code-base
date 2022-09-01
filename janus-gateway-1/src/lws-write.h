/*
  * libwebsockets - small server side websockets and web server implementation
  *
  * Copyright (C) 2010 - 2019 Andy Green <andy@warmcat.com>
  *
  * Permission is hereby granted, free of charge, to any person obtaining a copy
  * of this software and associated documentation files (the "Software"), to
  * deal in the Software without restriction, including without limitation the
  * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  * sell copies of the Software, and to permit persons to whom the Software is
  * furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  * IN THE SOFTWARE.
  */
  
 #if !defined(LWS_SIZEOFPTR)
 #define LWS_SIZEOFPTR ((int)sizeof (void *))
 #endif
  
 #if defined(__x86_64__)
 #define _LWS_PAD_SIZE 16        /* Intel recommended for best performance */
 #else
 #define _LWS_PAD_SIZE LWS_SIZEOFPTR   /* Size of a pointer on the target arch */
 #endif
 #define _LWS_PAD(n) (((n) % _LWS_PAD_SIZE) ? \
                 ((n) + (_LWS_PAD_SIZE - ((n) % _LWS_PAD_SIZE))) : (n))
 /* last 2 is for lws-meta */
 #define LWS_PRE _LWS_PAD(4 + 10 + 2)
 /* used prior to 1.7 and retained for backward compatibility */
 #define LWS_SEND_BUFFER_PRE_PADDING LWS_PRE
 #define LWS_SEND_BUFFER_POST_PADDING 0
  
 #define LWS_WRITE_RAW LWS_WRITE_HTTP
  
 /*
  * NOTE: These public enums are part of the abi.  If you want to add one,
  * add it at where specified so existing users are unaffected.
  */
 enum lws_write_protocol {
         LWS_WRITE_TEXT                                          = 0,
         LWS_WRITE_BINARY                                        = 1,
         LWS_WRITE_CONTINUATION                                  = 2,
         LWS_WRITE_HTTP                                          = 3,
         /* LWS_WRITE_CLOSE is handled by lws_close_reason() */
         LWS_WRITE_PING                                          = 5,
         LWS_WRITE_PONG                                          = 6,
  
         /* Same as write_http but we know this write ends the transaction */
         LWS_WRITE_HTTP_FINAL                                    = 7,
  
         /* HTTP2 */
  
         LWS_WRITE_HTTP_HEADERS                                  = 8,
         LWS_WRITE_HTTP_HEADERS_CONTINUATION                     = 9,
         /****** add new things just above ---^ ******/
  
         /* flags */
  
         LWS_WRITE_BUFLIST = 0x20,
         LWS_WRITE_NO_FIN = 0x40,
         LWS_WRITE_H2_STREAM_END = 0x80,
         LWS_WRITE_CLIENT_IGNORE_XOR_MASK = 0x80
 };
  
 /* used with LWS_CALLBACK_CHILD_WRITE_VIA_PARENT */
  
 struct lws_write_passthru {
         struct lws *wsi;
         unsigned char *buf;
         size_t len;
         enum lws_write_protocol wp;
 };
  
  
 LWS_VISIBLE LWS_EXTERN int
 lws_write(struct lws *wsi, unsigned char *buf, size_t len,
           enum lws_write_protocol protocol);
  
 /* helper for case where buffer may be const */
 #define lws_write_http(wsi, buf, len) \
         lws_write(wsi, (unsigned char *)(buf), len, LWS_WRITE_HTTP)
  
 static LWS_INLINE int
 lws_write_ws_flags(int initial, int is_start, int is_end)
 {
         int r;
  
         if (is_start)
                 r = initial;
         else
                 r = LWS_WRITE_CONTINUATION;
  
         if (!is_end)
                 r |= LWS_WRITE_NO_FIN;
  
         return r;
 }
  
 LWS_VISIBLE LWS_EXTERN int LWS_WARN_UNUSED_RESULT
 lws_raw_transaction_completed(struct lws *wsi);
  
