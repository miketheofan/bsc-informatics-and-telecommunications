# YS11-Data Mining

Projects for the course "_YS11-Data Mining_" 2022 by professor [Dimitrios Gounopoulos](https://www.di.uoa.gr/staff/46).

## Project 1

This project focuses on sentiment analysis of tweets related to COVID-19 vaccines using Python. The workflow includes:

1. **Data Preprocessing**: The dataset is cleaned by removing punctuation, converting text to lowercase, and eliminating unwanted symbols and stopwords.

2. **Data Analysis**: The analysis involves determining the distribution of sentiments (positive, negative, neutral), identifying common words, and comparing sentiments in subsets of tweets.

3. **Feature Extraction**: Features are extracted using techniques such as Bag-of-Words, TF-IDF, and word embeddings.

4. **Classifier Evaluation**: Various classifiers, including SVM, Random Forests, and KNN, are tested and evaluated based on the extracted features.

5. **Topic Modeling**: The LDA (Latent Dirichlet Allocation) algorithm is used to model topics, with the goal of determining the optimal number of topics based on coherence scores.

6. **Bonus**: Additional classification methods and features are explored to enhance performance, with results presented in a detailed Python notebook.

The project culminates in a comprehensive Python notebook that documents the entire process and presents the findings.

## Project 2

This project focuses on predicting taxi trip durations in New York City using data analysis and machine learning techniques. The objectives include:

1. **Data Preprocessing**:
   - Clean and preprocess the dataset by removing outliers based on `trip_duration` and filtering out data outside NYC geographical boundaries.
   - Convert datetime columns to appropriate formats and create new time-related features such as `day_period`, `day_name`, `month`, `hour`, `year`, and `season`.

2. **Data Analysis**:
   - Generate and visualize histograms for trip durations and other temporal features.
   - Analyze the impact of `passenger_count` and create features to calculate distances using Manhattan distance.

3. **Visualization**:
   - Create scatter plots for pickup and dropoff locations.
   - Plot average trip duration by various temporal dimensions (hour, day_period, day of week).

4. **Clustering**:
   - Apply K-Means clustering to pickup locations based on latitude and longitude.
   - Determine the optimal number of clusters and visualize the clusters and their centroids.

5. **Predictive Modeling**:
   - Use RandomForestRegressor to predict `log_trip_duration` based on various features.
   - Employ GridSearchCV to optimize model parameters and evaluate performance using MAE and MSE metrics.

6. **Bonus Tasks**:
   - Utilize the folium library to create a map of NYC showing pickup or dropoff points.
   - Experiment with additional features and analysis to further enhance model performance.

The final deliverable is an IPython notebook demonstrating all steps, including data analysis, clustering, and regression modeling, with visualizations and detailed commentary on observations and conclusions.

