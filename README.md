# YS19-Artificial Intelligence

Projects for the course "_YS19-Artificial Intelligence_" 2022 by professor [Manolis Koubarakis](https://cgi.di.uoa.gr/~koubarak/).

## Project 1

This assignment involves developing a sentiment classifier for movie reviews using logistic regression. The dataset provided, `imdb-reviews.csv`, includes IMDb movie URLs, scores, and review texts. The task is to classify reviews into two sentiment categories: negative (scores between 0.0 and 4.0) and positive (scores between 7.0 and 10.0). No intermediate scores are present in the dataset.

### Objectives:

1. **Data Preprocessing**:
   - Load and preprocess the dataset.
   - Handle text data for sentiment classification.

2. **Model Development**:
   - Implement a logistic regression model for sentiment classification.
   - Choose appropriate methods for data partitioning (training and validation sets), cross-validation, regularization, and hyperparameter tuning.

3. **Evaluation**:
   - Plot learning curves to ensure the model does not overfit or underfit.
   - Evaluate the classifier using precision, recall, and F-measure.
   - Utilize the Scikit-Learn library for implementation and evaluation.

4. **Documentation**:
   - Prepare a detailed PDF report explaining the solution, including citations to relevant literature. Bonus points for using LaTeX.
   - Submit a Colab notebook (ipynb file) containing the code and results.

The code should be designed for easy evaluation on a test set by specifying the path to the test file in a variable. Ensure that the approach is well-documented and reproducible.

## Project 2

## Project Summary

This assignment involves developing a sentiment classifier for the `imdb-reviews.csv` dataset from Homework 1 using feedforward neural networks implemented in PyTorch. The classifier should use GloVe word embeddings for input.

### Objectives:

1. **Data Preprocessing**:
   - Load the dataset and preprocess the text data.
   - Integrate GloVe word embeddings into the model.

2. **Model Development**:
   - Implement feedforward neural networks using PyTorch.
   - Experiment with various neural network architectures and hyperparameters to identify the most effective model.

3. **Evaluation**:
   - Plot learning curves to assess model fit (overfitting or underfitting).
   - Evaluate the model using precision, recall, and F-measure.
   - Plot ROC curves to evaluate the classifier's performance.

4. **Documentation**:
   - Prepare a detailed PDF report explaining the solution, including citations to relevant literature. Bonus points for using LaTeX (submit LaTeX source files if used).
   - Submit one or more Colab notebooks (ipynb files) containing the code and results.

The code should be designed to evaluate the model on a test set by specifying the test file path in a variable. Ensure comprehensive documentation and reproducibility of results.

## Project 3

## Project Summary

This assignment focuses on developing a sentiment classifier for the `imdb-reviews.csv` dataset from Homework 1 and 2 using bidirectional stacked RNNs with LSTM/GRU cells implemented in PyTorch. The classifier should incorporate GloVe word embeddings.

### Objectives:

1. **Model Development**:
   - Implement a bidirectional stacked RNN with LSTM or GRU cells.
   - Experiment with various configurations, including the number of stacked RNN layers, hidden layers, types of RNN cells, skip connections, gradient clipping, and dropout probability.
   - Use the Adam optimizer and the cross-entropy loss function.
   - Experiment with multiple neural network architectures to identify the most effective one.

2. **Evaluation**:
   - Plot learning curves to assess model fit (avoiding overfitting or underfitting).
   - Evaluate the model using precision, recall, F-measure, and ROC curves.
   - Compare the best model with the models developed in Homeworks 1 and 2.

3. **Documentation**:
   - Prepare a detailed PDF report explaining the solution, including citations to relevant literature. Bonus points for using LaTeX (submit LaTeX source files if used).
   - Submit one or more Colab notebooks (ipynb files) containing the code and results.

4. **Bonus**:
   - Incorporate the attention mechanism into the architecture for an additional 20% bonus.

The code should be structured to allow easy evaluation on the test set by specifying the test file path in a variable. Ensure thorough documentation and reproducibility of results.
