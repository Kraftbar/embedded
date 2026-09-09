from flask import Flask, request
from flask_cors import CORS
import socket

app = Flask(__name__)
CORS(app)

# Setup UDP socket
udp_ip = '10.131.29.45'  # Replace 'last_ip_here' with the actual IP address
udp_port = 12345
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

@app.route('/', methods=['POST'])
def receive_drawing():
    data = request.json
    print(f"Received coordinates: {data}")

    # Prepare a vector of 12 zeros
    controller_data = [0] * 12
    # Fill the first two elements with x and y coordinates
    controller_data[1] = data['x']
    controller_data[2] = data['y']

    # Convert the controller_data list to string and encode to bytes for UDP transmission
    message = f"{controller_data}\n".encode()

    # Send the data via UDP
    sock.sendto(message, (udp_ip, udp_port))
    return '', 200

if __name__ == "__main__":
    app.run(host='10.131.29.45', port=12355)

