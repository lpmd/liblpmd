/*
 *
 *
 *
 */

#ifndef __REGION_H__
#define __REGION_H__

class State; // Forward declaration

class Confined
{
 public:
   virtual bool Outside(const State & b1, const State & b2) const { return false; }

};

#endif

