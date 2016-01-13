/*
 * This file is part of Hootenanny.
 *
 * Hootenanny is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * --------------------------------------------------------------------
 *
 * The following copyright notices are generated automatically. If you
 * have a new notice to add, please use the format:
 * " * @copyright Copyright ..."
 * This will properly maintain the copyright information. DigitalGlobe
 * copyrights will be updated automatically.
 *
 * @copyright Copyright (C) 2015 DigitalGlobe (http://www.digitalglobe.com/)
 */
#include "NetworkMatchCreator.h"

// hoot
#include <hoot/core/Factory.h>
#include <hoot/core/OsmMap.h>
#include <hoot/core/conflate/MatchType.h>
#include <hoot/core/conflate/MatchThreshold.h>
#include <hoot/core/elements/ElementVisitor.h>
#include <hoot/core/filters/ChainCriterion.h>
#include <hoot/core/filters/HighwayCriterion.h>
#include <hoot/core/filters/StatusCriterion.h>
#include <hoot/core/schema/OsmSchema.h>
#include <hoot/core/util/NotImplementedException.h>
#include <hoot/core/util/ConfigOptions.h>
#include <hoot/rnd/conflate/network/IterativeNetworkMatcher.h>
#include <hoot/rnd/conflate/network/NetworkMatch.h>
#include <hoot/rnd/conflate/network/OsmNetworkExtractor.h>

// Standard
#include <fstream>

// tgs
#include <tgs/RandomForest/RandomForest.h>

namespace hoot
{

HOOT_FACTORY_REGISTER(MatchCreator, NetworkMatchCreator)

using namespace Tgs;

NetworkMatchCreator::NetworkMatchCreator()
{
}

Match* NetworkMatchCreator::createMatch(const ConstOsmMapPtr& /*map*/, ElementId /*eid1*/,
  ElementId /*eid2*/)
{
  Match* result = 0;

  return result;
}

void NetworkMatchCreator::createMatches(const ConstOsmMapPtr& map, vector<const Match*>& /*matches*/,
  ConstMatchThresholdPtr /*threshold*/)
{
  // use another class to extract graph nodes and graph edges.
  OsmNetworkExtractor e1;
  ElementCriterionPtr c1(new ChainCriterion(new StatusCriterion(Status::Unknown1),
    _userCriterion));
  e1.setCriterion(c1);
  OsmNetworkPtr n1 = e1.extractNetwork(map);

  OsmNetworkExtractor e2;
  ElementCriterionPtr c2(new ChainCriterion(new StatusCriterion(Status::Unknown2),
    _userCriterion));
  e2.setCriterion(c2);
  OsmNetworkPtr n2 = e2.extractNetwork(map);

  // call class to derive final graph node and graph edge matches
  IterativeNetworkMatcher matcher;
  matcher.matchNetworks(map, n1, n2);

  // convert graph edge matches into NetworkMatch objects.
  #warning todo
}

vector<MatchCreator::Description> NetworkMatchCreator::getAllCreators() const
{
  vector<Description> result;

  result.push_back(Description(className(), "Network Match Creator", true));

  return result;
}

bool NetworkMatchCreator::isMatchCandidate(ConstElementPtr /*element*/, const ConstOsmMapPtr& /*map*/)
{
  return false;
}

shared_ptr<MatchThreshold> NetworkMatchCreator::getMatchThreshold()
{
  if (!_matchThreshold.get())
  {
    ConfigOptions config;
    _matchThreshold.reset(
      new MatchThreshold(config.getPoiMatchThreshold(), config.getPoiMissThreshold(),
                         config.getPoiReviewThreshold()));
  }
  return _matchThreshold;
}

}