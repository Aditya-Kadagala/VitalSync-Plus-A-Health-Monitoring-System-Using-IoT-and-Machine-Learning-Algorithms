from flask import Flask, request, redirect, render_template
import pickle
from sklearn.preprocessing import StandardScaler
import pymysql 
import numpy as np
from twilio.rest import Client
from twilio.base.exceptions import TwilioRestException
account_sid = 'Twilio Acc'
auth_token = 'Twillio Token'
client = Client(account_sid, auth_token)
app = Flask(__name__, template_folder='templates')


def get_data_from_database():
    connection = pymysql.connect(host='localhost',
                                 user='root',
                                 password='',
                                 database='mini_project')
    cursor = connection.cursor()
    cursor.execute("SELECT temp, co2, hr, hrv, spo2, shr FROM mini_project_sensor_data ORDER BY id DESC LIMIT 1")
    data = cursor.fetchone()
    connection.close()
    return data

# Load the trained model    
model_load = pickle.load(open('pkkl_file.pkl', 'rb'))
#scaler = pickle.load(open('Scaler_file.pkl', 'rb'))

@app.route('/')
def hello_world():
    data = get_data_from_database()
    return render_template("prediction_page.html", data=data)

@app.route('/predict', methods=['POST'])
def predict():
    if request.method == 'POST':
        # Get input values from the form
        #input_features = [50, 1, 4, 130, 233, 0, 0, 121, 1, 2, 2]
        input_features=[x for x in request.form.values()]
        sms_features = input_features[0:6]
        input_features = input_features[6:]
        print(sms_features)
        print(input_features)
        input_features = [int(x) for x in input_features]
        input_features.append(0)
        zz = input_features[0]
        input_features=[np.array(input_features)]
        #input_features = [[54,1,2,120,246,0,0,110,0,0,1,0]]
        # Apply the same scaler used in training
        #scaled_features = scaler.transform([input_features])
        # Make prediction using the loaded model
        prediction = model_load.predict(input_features)
        print(prediction)
        print(model_load.predict_proba(input_features)[0][1])
        if prediction[0] == 1:
            sms_value2 = "\nYOU HAVE THE HEART DISEASE (PROBABILITY: {:.2f})".format(model_load.predict_proba(input_features)[0][1])
        else:
            sms_value2 = "\nYOU DOESN'T HAVE THE HEART DISEASE (PROBABILITY: {:.2f})".format(model_load.predict_proba(input_features)[0][1])
        sms_value1 = f"Here is Your Health Data:\n\
        Temperature:            {sms_features[1]} °C\n\
        Carbon Dioxide:         {sms_features[2]} PPM\n\
        Blood Oxygen Level:     {sms_features[3]} %\n\
        Heart Rate:             {sms_features[4]} BPM\n\
        Heart Rate Validator:   {sms_features[5]} ms\n\n"
        if(sms_features[0]=='0'):
            target_num = '+911234545'
        else:
            target_num = '+91'+sms_features[0]  
        print(target_num)
        if (zz == 0):
            sms_value = sms_value1
        else:
            sms_value = sms_value1+sms_value2
        try:
            message = client.messages.create(
                body=sms_value,
                from_='Number',
                to=target_num
            )
        except TwilioRestException as e:
            print("An error occurred while sending the message:", e)
        if prediction[0] == 1:
            res_pred = "YOU HAVE THE HEART DISEASE (PROBABILITY: {:.2f})".format(model_load.predict_proba(input_features)[0][1])
        else:
            res_pred = "YOU DON'T HAVE THE HEART DISEASE (PROBABILITY: {:.2f})".format(model_load.predict_proba(input_features)[0][1])

        # Pass the prediction result to the template
        return render_template('prediction_page.html', pred=res_pred, data=None)  # Passing None for 'data'
        #return render_template('prediction_page.html', pred="prediction[0]", data=None)  # Passing None for 'data'
if __name__ == '__main__':
    app.run(debug=True)
