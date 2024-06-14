from flask import Flask, request, jsonify

app = Flask(__name__)

# Data dummy untuk disimpan sementara
data = {"temperature": None, "humidity": None}

# Route untuk metode GET
@app.route('/dht/sensordata', methods=['GET'])
def get_data():
    return jsonify(data)

# Route untuk metode POST
@app.route('/dht/sensordata', methods=['POST'])
def post_data():
    content = request.json
    if 'temperature' in content and 'humidity' in content:
        data['temperature'] = content['temperature']
        data['humidity'] = content['humidity']
        return "data berhasil tersimpan di server", 200
    else:
        return "Incomplete data", 400

if __name__ == '__main__':
    app.run(debug=True)