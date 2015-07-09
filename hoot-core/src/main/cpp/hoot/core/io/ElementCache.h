#ifndef ELEMENTCACHE_H
#define ELEMENTCACHE_H

#include <boost/shared_ptr.hpp>
#include <hoot/core/elements/Element.h>
#include <hoot/core/elements/ElementId.h>
#include <hoot/core/elements/ElementProvider.h>
#include <hoot/core/elements/Node.h>
#include <hoot/core/elements/Way.h>
#include <hoot/core/elements/Relation.h>
#include <hoot/core/io/ElementInputStream.h>
#include <hoot/core/io/ElementOutputStream.h>

#include <ogr_spatialref.h>


namespace hoot
{

/**
 * Class description
 */
class ElementCache : public ElementInputStream, public ElementOutputStream, public ElementProvider
{

public:

  /**
   * @brief ElementCache
   */
  ElementCache() { }

  /**
   * @brief ~ElementCache
   */
  virtual ~ElementCache() { }

  /**
   * @brief isEmpty
   * @return True if the cache does not contain any elements, else false
   */
  virtual bool isEmpty() const = 0;

  /**
   * @brief size
   * @return Total number of elements in the cache
   */
  virtual unsigned long size() const = 0;

  /**
   * @brief addElement
   *
   * Add a new element into the cache. If the item already exists in the cache, the operation
   *    should be treated as a no-op
   *
   * @note This function should be overriden in child classes that want to implement a
   *    cache management strategy, such as least-recently used (LRU), etc.
   *
   * @note This method will invoke ResetIterators() before it returns (i.e., every time a new
   *    element is added to the cache, all iterators are reset to start of their respective lists)
   *
   * @param newElement Element to add to cache
   */
  virtual void addElement(ConstElementPtr& newElement) = 0;

  /**
   * @brief ResetIterators
   *
   * Causes the cache to reset its iterators for nodes, ways, and relations to the start of
   *    each list
   */
  virtual void resetElementIterators() = 0;

  /**
   * @brief getNextNode
   * @return Pointer to next entry in cache's list of nodes, or ConstNodePtr() if no more nodes
   *
   * @note Nodes will be returned in order by ascending node ID
   */
  virtual ConstNodePtr getNextNode() = 0;

  /**
   * @brief getNextWay
   * @return Pointer to the next entry in cache's list of ways, or ConstWayPtr() if no more ways
   *
   * @note Ways will be returned in order by ascending way ID
   */
  virtual ConstWayPtr getNextWay() = 0;

  /**
   * @brief getNextRelation
   * @return Pointer to the next entry in cache's list of relations, or ConstRelationPtr() if no
   *      more ways can be read
   *
   * @note Relations will be returned in order by ascending relation ID
   */
  virtual ConstRelationPtr getNextRelation() = 0;

  // Functions for ElementInputStream
  virtual void close() = 0;             // Also works for elementoutputstream
  virtual bool hasMoreElements() = 0;

  /**
   * @brief readNextElement
   *
   * @note There is no guarantee of the order that items will be pulled from the cache when this
   *      method is invoked. If ordering of elements (e.g., all nodes first, then all ways, and
   *      finally all relations) is required, the getNext(Node/Way/Relation) methods should be
   *      invoked
   *
   * @return Pointer to next element out of cache
   */
  virtual ElementPtr readNextElement() = 0;

  // Functions for ElementOutputStream
  virtual void writeElement(ElementInputStream& inputStream) = 0;



  // Functions from ElementProvider

  virtual boost::shared_ptr<OGRSpatialReference> getProjection() const = 0;

  virtual bool containsElement(const ElementId& eid) const = 0;

  virtual ConstElementPtr getElement(const ElementId& id) const = 0;

  virtual const boost::shared_ptr<const Node> getNode(long id) const = 0;

  virtual const boost::shared_ptr<Node> getNode(long id) = 0;

  virtual const boost::shared_ptr<const Relation> getRelation(long id) const = 0;

  virtual const boost::shared_ptr<Relation> getRelation(long id) = 0;

  virtual const boost::shared_ptr<const Way> getWay(long id) const = 0;

  virtual const boost::shared_ptr<Way> getWay(long id) = 0;

  virtual bool containsNode(long id) const = 0;

  virtual bool containsRelation(long id) const = 0;

  virtual bool containsWay(long id) const = 0;
};

typedef boost::shared_ptr<ElementCache> ElementCachePtr;

}

#endif // ELEMENTCACHE_H