# WARNING!

Project is under heavy development!

# NGX - New Engine Ten
[![Build Status](https://travis-ci.org/masscry/ngx.svg?branch=master)](https://travis-ci.org/masscry/ngx)
<a href="https://scan.coverity.com/projects/masscry-ngx">
  <img alt="Coverity Scan Build Status"
       src="https://scan.coverity.com/projects/12012/badge.svg"/>
</a>

Project consists of several low-level algoritms for general game development 
and provides test and examples of their usage in form of turn-base tactical 
strategy game.

## Overview

This is pet project, main idea is to develop several solutions for common task
appearing in indie game development. Tools are built with crossplatform in mind.

## Language

Main language is plain C99.

## Standard way to code

Nevertheless I am using C99 as main language, the code is defined in terms of 
"entities" AKA classes and objects.

Headers declares only pointers to structures and all functions related to 
this structure get a pointer as first argument. Actual structures are defined
inside its' respective compilation units.

```c
// entity.h
typedef struct ngx_entity_t* NGXENTITY;

NGXENTITY ngxEntityInit(); // Standard way to create new object

void ngxEntityCleanup(NGXENTITY* entity); // Standard way to destroy object. 
                                          // Cleanup function zeroes pointer.

// entity.c
struct ngx_entity_t {
  // Actual data definitions.
};

```

## NGXDEVICE

Rendering device.

## NGXARC

Data archive. Archive supports block sizes up to USHRT_MAX and total block count 
up to (USHRT_MAX-1). 0xFFFF block is used as error.

## NGXBLK

Single data block. Archive format supports a lot of block sizes, but now I am 
using only blocks of 512 bytes. The first two bytes are used as pointer to 
the next block in data chain. If equals to 0xFFFF, than current block is 
the last block in chain.

## NGXINDEX

Linear array of unique key-value pairs. Used to store data names and block ids

## NGXMODEL

Simple binary model file.
