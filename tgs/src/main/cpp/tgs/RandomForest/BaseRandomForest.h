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
#ifndef BASERANDOMFOREST_H
#define BASERANDOMFOREST_H

//Boost Includes
#include <boost/shared_ptr.hpp>

// Qt includes
#include <QFile>

#include "DataFrame.h"
#include "RandomTree.h"

namespace Tgs
{
  /**
   * @brief The TrainingInputs struct
   *
   * Contains the inputs required for multiclass training
   */
  struct TrainingInputs
  {
    TrainingInputs()
    {
      numFactors = 0;
      nodeSize = 1;
      balanced = false;
    }

    boost::shared_ptr<DataFrame> data;
    unsigned int numFactors;
    unsigned int nodeSize;
    bool balanced;
  };

  /**
   * @brief The BaseRandomForest class base class for the Random Forest
   *
   *  The RandomForest is an implementation of the Leo Breiman
   * Random Forest Classification algorithm
   *
   * For more information see:
   * http://oz.berkeley.edu/~breiman/RandomForests/
   *
   */
  class BaseRandomForest
  {
  public:
    /**
     * @brief BaseRandomForest constructor
     */
    BaseRandomForest();

    /**
     * @brief ~BaseRandomForest destructor
     */
    virtual ~BaseRandomForest();

    /**
    *  Clears the random forest of its trees.
    */
    void clear();

    /**
    * Classifies a data vector against a generated random forest.
    *
    * The vector is classified against each tree in the forest and the final classification is the
    * result of majority vote for each tree.
    *
    * @param dataVector a single data vector of size N where N is the number of factors
    * @param scores a map to hold the classification results as class name mapped to probability
    */
    void classifyVector(std::vector<double> & dataVector, std::map<std::string, double> & scores)
      const;

    /**
    *  Exports the random forest to a file
    *
    * @param modelDoc the active DOM document
    * @param parentNode the parent node to append the forest
    */
    void exportModel(QDomDocument & modelDoc, QDomElement & parentNode);

    /**
     * Exports the random forest to a file
     *
     * @param fileStream Opened filestream for writing the random forest to a file; caller is
     * responsible for closing the stream
     */
    void exportModel(std::ostream& filestream);

    /**
    * Finds the average classification error statistic for the forest based on the
    * oob sets for the trees
    *
    * @param data the data set to operate upon
    * @param average variable to hold the computed average error
    * @param stdDev variable to hold the computed standard deviation
    */
    void findAverageError(boost::shared_ptr<DataFrame> data, double & average, double & stdDev);

    /**
    * Computes the proximity of the data vectors in the data set by running the
    * complete data set through the tree and then tracking which vectors
    * were classified to the same node
    *
    * @param data the set of data vectors
    * @param proximity a n x n (where n is the number of total data vectors) adjacency matrix
    */
    void findProximity(boost::shared_ptr<DataFrame> data, std::vector<unsigned int> & proximity);

    /**
    * This generates a text file containing the raw probability scores and a text file
    * containing the confusion matrix along with generated balanced *accuracy
    */
    void generateResults(std::string fileName);

    /**
    *  Gets the factor importance as generated by the random forest the highest
    *  values correspond to most important factors.
    *
    *  @param data the original data set
    *  @param factorImportance a map of factor labels to purity improvement
    */
    void getFactorImportance(boost::shared_ptr<DataFrame> data,
      std::map<std::string, double> & factorImportance);

    /**
     * Return a vector of the factor labels used to train this random forest.
     */
    const vector<string>& getFactorLabels() const { return _factorLabels; }

    /**
     * @brief importModel import the random forest object
     * @param e the XML DOM element for the forest
     */
    void importModel(QDomElement & e);

    /**
     * Import the random forest object
     *
     * @param file an opened file handle to a random forest (.rf) file; caller is responsible for
     * closing the handle
     */
    void importModel(QFile& file);

    /**
    *  @return true if the forest has been trained
    */
    bool isTrained(){return _forestCreated;}

    /**
     * @brief replaceMissingTrainingValues
     *
     * Replaces null values in the training data with the median values by class
     *
     * Should be used after all training data is applied and before a model is generated
     */
    void replaceMissingTrainingValues();


    /**
    * Build the forest from a data set
    *
    * @param data the data set to train on
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param posClass the name of the positive class
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the forest will be balanced
    */
    virtual void trainBinary(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
      unsigned int numFactors, std::string posClass, unsigned int nodeSize = 1,
      double retrain = 1.0, bool balanced = false) = 0;

    /**
    * Build the forest from a data set
    *
    * @param data the data set to train on
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the forest will be balanced
    */
    virtual void trainMulticlass(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
      unsigned int numFactors, unsigned int nodeSize = 1, double retrain = 1.0,
      bool balanced = false) = 0;

    /**
    * Build the forest from a data set
    *
    * @param data the data set to train on
    * @param numTrees the number of random trees to create
    * @param numFactors the number of factors to randomly choose as candidates for node splitting
    * @param posClass the name of the positive class
    * @param negClass the name of the negative class
    * @param nodeSize the minimum number of data vectors in a set to split a node
    * @param retrain fraction of top factors to use in retraining model (1.0 means use all factors and no retraining)
    * @param balanced true if the forest will be balanced
    */
    virtual void trainRoundRobin(boost::shared_ptr<DataFrame> data, unsigned int numTrees,
      unsigned int numFactors, std::string posClass, std::string negClass,
      unsigned int nodeSize = 1, double retrain = 1.0, bool balanced = false) = 0;

  protected:
    std::vector<boost::shared_ptr<RandomTree> > _forest; /// A container for the random forest

    unsigned int _numSplitFactors;  /// The number of factors to test to split a node
    unsigned int _nodeSize;  /// The minimum number of data vectors in a set to split a node
    vector<std::string> _factorLabels; /// Labels for all the factors used in training.

    static TrainingInputs _trainInputs;  ///The inputs used by the map function train

    bool _forestCreated;  /// A flag to indicate if the forest was created successfully

  };
}

#endif // BASERANDOMFOREST_H