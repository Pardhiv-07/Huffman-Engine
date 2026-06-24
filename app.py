import os;
import subprocess;
from flask import Flask, request, send_file ,render_template

#Initializing the flask application
app = Flask(__name__)

#Creating a temp folder to handle files securely
UPLOAD_FOLDER = 'uploads'
os.makedirs(UPLOAD_FOLDER, exist_ok=True)

#Path to the compressor executable
if os.name == 'nt':  # Windows
    COMPRESSOR_EXE = os.path.join('.', 'build', 'compressor.exe')
else:  # Linux/macOS
    COMPRESSOR_EXE = os.path.join('.', 'build', 'compressor')

@app.route('/')
def index():
    # Serving the HTML dashboard when a user visits the site
    return render_template('index.html', stats=None)

@app.route('/process', methods=['POST'])

def process_file():
    # 1. Checking if the user actually uploaded a file
    if 'file' not in request.files:
        return "Error: No file uploaded.", 400
    
    file = request.files['file']
    if file.filename == '':
        return "Error: No file selected.", 400
    
    # 2. Getting the action from the web form ('compress' or 'decompress')
    action = request.form.get('action')

    # 3. Saving the uploaded file to our temporary folder
    input_path = os.path.join(UPLOAD_FOLDER, file.filename)
    file.save(input_path)

    # 4. Generating the correct output filename
    if action == 'compress':
        output_filename = file.filename + '.bin'
    else: 
        output_filename = 'restored_' + file.filename.replace('.bin', '')
    
    output_path = os.path.join(UPLOAD_FOLDER, output_filename)

    # 5. Executing the C++ backend
    try:
        print(f"[Python] Sending command to C++: {COMPRESSOR_EXE} {action} {input_path} {output_path}")

        # Running the C++ executable 
        subprocess.run([COMPRESSOR_EXE, action, input_path, output_path], check=True)
    except subprocess.CalledProcessError as e:
        return f"Error: C++ Backend failed to process the file. Details: {e}", 500
    

    input_size = os.path.getsize(input_path)
    output_size = os.path.getsize(output_path)

    if action == 'compress' and input_size > 0:
        ratio = f"{((1 - (output_size / input_size)) * 100):.2f}% Space Saved"
    else:
        ratio = "100% Restored"

    # Package the stats into a Python dictionary
    stats = {
        'original': f"{input_size} Bytes",
        'processed': f"{output_size} Bytes",
        'ratio': ratio,
        'filename': output_filename
    }

    # Reload the page, but inject the stats data!
    return render_template('index.html', stats=stats)

# --- NEW: Dedicated Download Route ---
@app.route('/download/<filename>')
def download_file(filename):
    file_path = os.path.join(UPLOAD_FOLDER, filename)
    return send_file(file_path, as_attachment=True)
if __name__ == '__main__':
    # Starting the local development server
    app.run(debug=True)
