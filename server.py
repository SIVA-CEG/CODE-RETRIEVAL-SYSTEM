import subprocess
from flask import Flask, request, render_template, jsonify

app = Flask(__name__)

@app.route('/')
def home():
    return render_template('index.html')

@app.route('/getcode', methods=['GET'])
def get_code():
    description = request.args.get('description')

    result = subprocess.run(['./crs.exe', 'getcode', description], capture_output=True, text=True)

    return jsonify({'result': result.stdout.strip()})

@app.route('/suggest', methods=['GET'])
def suggest():
    prefix = request.args.get('prefix')

    result = subprocess.run(['./crs.exe', 'suggest', prefix], capture_output=True, text=True)

    suggestions = result.stdout.strip().split('\n')  
    return jsonify(suggestions)

if __name__ == '__main__':
    app.run(debug=True)
