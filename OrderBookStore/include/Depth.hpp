/*
 * Depth.hpp
 *
 *  Created on: 09-Jun-2016
 *      Author: ankithbti
 */

#ifndef INCLUDE_DEPTH_HPP_
#define INCLUDE_DEPTH_HPP_

#include <Common.hpp>
#include <DepthLevel.hpp>

namespace obLib
{

  template <size_t SIZE = 10>
  struct Depth
  {

    DepthLevel _levels[SIZE*2]; // To Store Bid & Ask Depth Levels

    typedef std::map<Price, DepthLevel, std::greater<Price> > ExcessBidLevelContainer;
    typedef std::map<Price, DepthLevel, std::less<Price> > ExcessAskLevelContainer;

    ExcessBidLevelContainer _bidsExcessLevelContainer;
    ExcessAskLevelContainer _asksExcessLevelContainer;
    ChangeId _lastChanged;
    ChangeId _lastPublished;

    DepthLevel* getBidFirstLevel() {
      return _levels;
    }

    DepthLevel* getBidLastLevel() {
      return _levels + (SIZE - 1);
    }

    DepthLevel* getAskFirstLevel() {
      return _levels + (SIZE);
    }

    DepthLevel* getAskLastLevel() {
      return _levels + (SIZE*2 - 1);
    }

    const DepthLevel* getBidFirstLevel() const {
      return _levels;
    }

    const DepthLevel* getBidLastLevel() const {
      return _levels + (SIZE - 1);
    }

    const DepthLevel* getAskFirstLevel() const {
      return _levels + (SIZE);
    }

    const DepthLevel* getAskLastLevel() const {
      return _levels + (SIZE*2 - 1);
    }


    void add_order(Price price, Quantity qty, bool isBid){
      DepthLevel* level = findLevel(price, isBid);
      if(level){
	  level->add_order(qty);
	  if(!level->_isExcess){
	      // Depth changes should be published
	      ++_lastChanged;
	      level->last_change(_lastChanged);
	  }else{
	      // No need to publish
	  }
      }
    }

    bool replace_order(Price oldPrice, Price newPrice, Quantity oldQty, Quantity newQty, bool isBid){
      bool erased = false;
      // If the price is unchanged, modify this level only
      if(oldPrice == newPrice){
	  Quantity delta = newQty - oldQty;
	  changeQtyOrder(newPrice, delta, isBid);
      }else{
	  add_order(newPrice, newQty, isBid);
	  erased = close_order(oldPrice, oldQty, isBid);
      }
      return erased;
    }

    void changeQtyOrder(Price price, Quantity delta, bool isBid){
      DepthLevel* level = findLevel(price, isBid, false);
      if(level && delta){
	  if(delta > 0){
	      level->increase_qty(delta);
	  }else{
	      level->decrease_qty(std::abs(delta));
	  }
	  level->last_change(++_lastChanged);
      }
    }

    bool close_order(Price price, Quantity open_qty, bool isBid){
      DepthLevel* level = findLevel(price, isBid, false);
      if(level){
	  if(level->close_order(open_qty)){
	      // This was the last order on that Level
	      erase_level(level, isBid);
	      return true;
	  }else{
	      level->last_change(++_lastChanged);
	  }
      }
      return false;
    }



    bool changed() const{
      return _lastChanged > _lastPublished;
    }

    ChangeId last_change() const{
      return _lastChanged;
    }

    ChangeId last_published_change() const{
      return _lastPublished;
    }

    void published(){
      _lastPublished = _lastChanged;
    }





  private:

    DepthLevel* findLevel(Price price, bool is_bid, bool should_create = true){
      DepthLevel* levelIt = is_bid ? getBidFirstLevel() : getAskFirstLevel();
      DepthLevel* end = is_bid ? getBidLastLevel() : getAskLastLevel();

      // Linear search each level
      for(;levelIt != end ; ++levelIt){
	  if(levelIt->_price == price){
	      break;
	  }
	  else if(should_create && levelIt->_price == INVALID_LEVEL_PRICE){
	      // First time we are adding in this level
	      levelIt->init(price, false);
	      break;
	  }else if(is_bid && should_create && levelIt->_price < price){
	      // Insert a new slot for it as it is a new Best Bid
	      insert_level_before(levelIt, is_bid, price);
	      break;
	  }else if( (!is_bid) && should_create && levelIt->_price > price){
	      // Insert a new slot for it as it is a new Best Ask
	      insert_level_before(levelIt, is_bid, price);
	      break;
	  }
      }


      if(levelIt == end){
	  // might be in excessLevels
	  if(is_bid){
	      ExcessBidLevelContainer::iterator it = _bidsExcessLevelContainer.find(price);
	      if(it != _bidsExcessLevelContainer.end()){
		  levelIt = &(it->second);
	      }else if(should_create){
		  DepthLevel newLevel;
		  newLevel.init(price, true);
		  std::pair<ExcessBidLevelContainer::iterator, bool> retVal = _bidsExcessLevelContainer.insert(std::pair<Price, DepthLevel>(price, newLevel));
		  levelIt = &(retVal.first->second);
	      }
	  }else{
	      ExcessAskLevelContainer::iterator it = _asksExcessLevelContainer.find(price);
	      if(it != _asksExcessLevelContainer.end()){
		  levelIt = &(it->second);
	      }else if(should_create){
		  DepthLevel newLevel;
		  newLevel.init(price, true);
		  std::pair<ExcessAskLevelContainer::iterator, bool> retVal = _asksExcessLevelContainer.insert(std::pair<Price, DepthLevel>(price, newLevel));
		  levelIt = &(retVal.first->second);
	      }
	  }
      }
      return levelIt;
    }


    void insert_level_before(DepthLevel* level, bool isBid, Price price){
      DepthLevel* lastLevel = isBid ? getBidLastLevel() : getAskLastLevel();

      if(lastLevel->_price != INVALID_LEVEL_PRICE){
	  DepthLevel excessLevel;
	  excessLevel.init(0, true);
	  excessLevel = *lastLevel;
	  if(isBid){
	      _bidsExcessLevelContainer.insert(std::make_pair(lastLevel->_price, excessLevel));
	  }else{
	      _asksExcessLevelContainer.insert(std::make_pair(lastLevel->_price, excessLevel));
	  }
      }

      // Shift the levels down
      DepthLevel * currLevel = lastLevel - 1;
      ++_lastChanged;
      while(currLevel >= level){
	  // Copy the level to move it one place down
	  *(currLevel + 1) = *currLevel;
	  if(currLevel->_price != INVALID_LEVEL_PRICE){
	      // Update Change Id
	      (currLevel + 1)->last_change(_lastChanged);
	  }
	  --currLevel;
      }
      level->init(price, false);
    }

    void erase_level(DepthLevel* level, bool isBid){
      if(level->_isExcess){
	  // Remove from excess map
	  if(isBid){
	      _bidsExcessLevelContainer.erase(level->_price);
	  }else{
	      _asksExcessLevelContainer.erase(level->_price);
	  }
      }else{
	  DepthLevel* lastSideLevel = isBid ? getBidLastLevel() : getAskLastLevel();
	  ++_lastChanged;
	  DepthLevel* currLevel = level;
	  while(currLevel < lastSideLevel){
	      if( (currLevel->_price != INVALID_LEVEL_PRICE) || (currLevel == level) ){
		  *(currLevel) = *(currLevel+1);
		  currLevel->last_change(_lastChanged);
	      }
	      ++currLevel;
	  }

	  if( (level == lastSideLevel) || (lastSideLevel->_price != INVALID_LEVEL_PRICE) ){
	      if(isBid){
		  ExcessBidLevelContainer::iterator it = _bidsExcessLevelContainer.begin();
		  if(it != _bidsExcessLevelContainer.end()){
		      *lastSideLevel = it->second;
		      _bidsExcessLevelContainer.erase(it);
		  }else{
		      // Nothing can be done.
		      lastSideLevel->init(INVALID_LEVEL_PRICE, false);
		      lastSideLevel->last_change(_lastChanged);
		  }
	      }else{
		  ExcessAskLevelContainer::iterator it = _asksExcessLevelContainer.begin();
		  if(it != _asksExcessLevelContainer.end()){
		      *lastSideLevel = it->second;
		      _asksExcessLevelContainer.erase(it);
		  }else{
		      // Nothing can be done.
		      lastSideLevel->init(INVALID_LEVEL_PRICE, false);
		      lastSideLevel->last_change(_lastChanged);
		  }
	      }
	      lastSideLevel->last_change(_lastChanged);
	  }
      } // Not in Excess Level
    }




  };
}


#endif /* INCLUDE_DEPTH_HPP_ */
