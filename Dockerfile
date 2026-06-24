# 1. Use 'bookworm', the absolute latest and most secure version of Debian Linux
FROM python:3.10-slim-bookworm

# 2. Tell Linux to update its package list, UPGRADE all existing packages to patch vulnerabilities, and then install C++
RUN apt-get update && apt-get upgrade -y && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

# 3. Create a working directory inside the container
WORKDIR /app

# 4. Copy all your files from your laptop into the container
COPY . /app

# 5. Install Flask
RUN pip install flask gunicorn

# 6. Build the C++ Engine for Linux! (This replaces your Windows .exe)
RUN mkdir -p build && cd build && cmake .. && cmake --build .

# 7. Expose the port the web server will run on
EXPOSE 10000

# 8. Start the server using Gunicorn (a production-grade server, better than Flask's default)
CMD ["gunicorn", "-b", "0.0.0.0:10000", "app:app"]
