/*-
 * Copyright 2009 Colin Percival, 2011 ArtForz
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file was originally written by Colin Percival as part of the Tarsnap
 * online backup system.
 */

#include "config.h"
#include "miner.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>


#include "sph/sph_blake.h"
#include "sph/sph_bmw.h"
#include "sph/sph_groestl.h"
#include "sph/sph_jh.h"
#include "sph/sph_keccak.h"
#include "sph/sph_skein.h"
#include "sph/sph_luffa.h"
#include "sph/sph_cubehash.h"
#include "sph/sph_shavite.h"
#include "sph/sph_simd.h"
#include "sph/sph_echo.h"
#include "sph/sph_hamsi.h"
#include "sph/sph_fugue.h"
#include "sph/sph_shabal.h"
#include "sph/sph_whirlpool.h"
#include "sph/sph_gost.h"
#include "sph/sph_haval.h"

/* Move init out of loop, so init once externally, and then use one single memcpy with that bigger memory block */
typedef struct {
  sph_blake512_context    blake1;
  sph_bmw512_context      bmw1;
  sph_groestl512_context  groestl1;
  sph_skein512_context    skein1;
  sph_jh512_context       jh1;
  sph_keccak512_context   keccak1;
  sph_luffa512_context    luffa1;
  sph_cubehash512_context cubehash1;
  sph_shavite512_context  shavite1;
  sph_simd512_context     simd1;
  sph_echo512_context     echo1;
  sph_hamsi512_context    hamsi1;
  sph_fugue512_context    fugue1;
  sph_shabal512_context   shabal1;
  sph_whirlpool_context   whirlpool1;
  sph_gost512_context     gost1;
  sph_haval256_5_context  haval1;
} Xhash_context_holder;

static Xhash_context_holder base_contexts;

void init_aergohash_contexts()
{
  sph_blake512_init(&base_contexts.blake1);
  sph_bmw512_init(&base_contexts.bmw1);
  sph_groestl512_init(&base_contexts.groestl1);
  sph_skein512_init(&base_contexts.skein1);
  sph_jh512_init(&base_contexts.jh1);
  sph_keccak512_init(&base_contexts.keccak1);
  sph_luffa512_init(&base_contexts.luffa1);
  sph_cubehash512_init(&base_contexts.cubehash1);
  sph_shavite512_init(&base_contexts.shavite1);
  sph_simd512_init(&base_contexts.simd1);
  sph_echo512_init(&base_contexts.echo1);
  sph_hamsi512_init(&base_contexts.hamsi1);
  sph_fugue512_init(&base_contexts.fugue1);
  sph_shabal512_init(&base_contexts.shabal1);
  sph_whirlpool_init (&base_contexts.whirlpool1);
  sph_gost512_init(&base_contexts.gost1);
  sph_haval256_5_init(&base_contexts.haval1);
}

void aergohash(void *state, const void *input)
{
  init_aergohash_contexts();

  Xhash_context_holder ctx;

  uint32_t hash[32];

  memcpy(&ctx, &base_contexts, sizeof(base_contexts));

  sph_echo512(&ctx.echo1, input, 80);
  sph_echo512_close(&ctx.echo1, hash);

  sph_simd512(&ctx.simd1, hash, 64);
  sph_simd512_close(&ctx.simd1, hash);

  sph_blake512 (&ctx.blake1, hash, 64);
  sph_blake512_close (&ctx.blake1, hash);

  sph_bmw512 (&ctx.bmw1, hash, 64);
  sph_bmw512_close(&ctx.bmw1, hash);

  sph_whirlpool(&ctx.whirlpool1, hash, 64);
  sph_whirlpool_close(&ctx.whirlpool1, hash);

  sph_groestl512 (&ctx.groestl1, hash, 64);
  sph_groestl512_close(&ctx.groestl1, hash);

  sph_gost512(&ctx.gost1, hash, 64);
  sph_gost512_close(&ctx.gost1, hash);

  sph_skein512 (&ctx.skein1, hash, 64);
  sph_skein512_close(&ctx.skein1, hash);

  sph_bmw512(&ctx.bmw1, hash, 64);
  sph_bmw512_close(&ctx.bmw1, hash);

  sph_jh512(&ctx.jh1, hash, 64);
  sph_jh512_close(&ctx.jh1, hash);

  sph_luffa512(&ctx.luffa1, hash, 64);
  sph_luffa512_close(&ctx.luffa1, hash);

  sph_keccak512 (&ctx.keccak1, hash, 64);
  sph_keccak512_close(&ctx.keccak1, hash);

  sph_gost512(&ctx.gost1, hash, 64);
  sph_gost512_close(&ctx.gost1, hash);

  sph_cubehash512 (&ctx.cubehash1, hash, 64);
  sph_cubehash512_close(&ctx.cubehash1, hash);

  sph_echo512(&ctx.echo1, hash, 64);
  sph_echo512_close(&ctx.echo1, hash);

  sph_simd512(&ctx.simd1, hash, 64);
  sph_simd512_close(&ctx.simd1, hash);

  sph_hamsi512(&ctx.hamsi1, hash, 64);
  sph_hamsi512_close(&ctx.hamsi1, hash);

  sph_fugue512(&ctx.fugue1, hash, 64);
  sph_fugue512_close(&ctx.fugue1, hash);

  sph_shavite512 (&ctx.shavite1, hash, 64);
  sph_shavite512_close(&ctx.shavite1, hash);

  sph_shabal512 (&ctx.shabal1, hash, 64);
  sph_shabal512_close(&ctx.shabal1, hash);

  sph_haval256_5(&ctx.haval1, hash, 64);
  sph_haval256_5_close(&ctx.haval1, hash);

  sph_shavite512(&ctx.shavite1, hash, 64);
  sph_shavite512_close(&ctx.shavite1, hash);

  sph_gost512(&ctx.gost1, hash, 64);
  sph_gost512_close(&ctx.gost1, hash);

  sph_echo512(&ctx.echo1, hash, 64);
  sph_echo512_close(&ctx.echo1, hash);

  sph_blake512(&ctx.blake1, hash, 64);
  sph_blake512_close(&ctx.blake1, hash);

  sph_jh512(&ctx.jh1, hash, 64);
  sph_jh512_close(&ctx.jh1, hash);

  sph_cubehash512(&ctx.cubehash1, hash, 64);
  sph_cubehash512_close(&ctx.cubehash1, hash);

  sph_simd512(&ctx.simd1, hash, 64);
  sph_simd512_close(&ctx.simd1, hash);

  memcpy(state, hash, 32);
}

static const uint32_t diff1targ = 0x0000ffff;

/* Used externally as confirmation of correct OCL code */
int aergo_test(unsigned char *pdata, const unsigned char *ptarget, uint32_t nonce)
{
  uint32_t tmp_hash7, Htarg = le32toh(((const uint32_t *)ptarget)[7]);
  uint32_t data[20], ohash[8];

  be32enc_vect(data, (const uint32_t *)pdata, 19);
  data[19] = htobe32(nonce);
  aergohash(ohash, data);
  tmp_hash7 = be32toh(ohash[7]);

  applog(LOG_DEBUG, "htarget %08lx diff1 %08lx hash %08lx",
        (long unsigned int)Htarg,
        (long unsigned int)diff1targ,
        (long unsigned int)tmp_hash7);

  if (tmp_hash7 > diff1targ)
    return -1;

  if (tmp_hash7 > Htarg)
    return 0;

  return 1;
}

void aergo_regenhash(struct work *work)
{
  uint32_t data[20];
  uint32_t *nonce = (uint32_t *)(work->data + 76);
  uint32_t *ohash = (uint32_t *)(work->hash);

  be32enc_vect(data, (const uint32_t *)work->data, 19);
  data[19] = htobe32(*nonce);
  aergohash(ohash, data);
}

bool scanhash_aergo(struct thr_info *thr, const unsigned char __maybe_unused *pmidstate,
         unsigned char *pdata, unsigned char __maybe_unused *phash1,
         unsigned char __maybe_unused *phash, const unsigned char *ptarget,
         uint32_t max_nonce, uint32_t *last_nonce, uint32_t n)
{
  uint32_t *nonce = (uint32_t *)(pdata + 76);
  uint32_t data[20];
  uint32_t tmp_hash7;
  uint32_t Htarg = le32toh(((const uint32_t *)ptarget)[7]);
  bool ret = false;

  be32enc_vect(data, (const uint32_t *)pdata, 19);

  while(1)
  {
    uint32_t ostate[8];
    *nonce = ++n;
    data[19] = (n);
	aergohash(ostate, data);
    tmp_hash7 = (ostate[7]);

    applog(LOG_INFO, "data7 %08lx", (long unsigned int)data[7]);

    if(unlikely(tmp_hash7 <= Htarg))
    {
      ((uint32_t *)pdata)[19] = htobe32(n);
      *last_nonce = n;
      ret = true;
      break;
    }

    if (unlikely((n >= max_nonce) || thr->work_restart))
    {
      *last_nonce = n;
      break;
    }
  }

  return ret;
}
