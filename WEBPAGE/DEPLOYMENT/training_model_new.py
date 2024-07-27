import warnings
import pickle
warnings.filterwarnings("ignore")
import numpy as np
import pandas as pd 
df=pd.read_csv('DEPLOYMENT/IEEE dataport dataset (2).csv')
from sklearn.ensemble import GradientBoostingClassifier
from sklearn.datasets import make_classification
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
X, y = make_classification(n_samples=918, n_features=12, random_state=42)
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)
model_new = GradientBoostingClassifier(n_estimators=100, learning_rate=0.1, random_state=42)
model_new.fit(X_train, y_train)
y_pred = model_new.predict(X_test)
accuracy = accuracy_score(y_test, y_pred)
print("Accuracy:", accuracy)
X_new = [[54,1,3,150,195,0,0,122,0,0.0,1,0]] 
predicted_label = model_new.predict(X_new)
print("Predicted Label:", predicted_label)

pickle.dump(model_new,open('model_new_new.pkl','wb'))
#pickle.dump(scaler_new, open('scaler_new.pkl', 'wb'))
model_load=pickle.load(open('model_new_new.pkl','rb'))