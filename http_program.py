import socket
import sys

# url portion
def parse_url(url):
    parts = url.split('/', 1)
    host = parts[0]
    path = '/' + parts[1] if len(parts) > 1 else '/'
    return host, path

# creates GET request
def create_http_request(host, path):
    request = f"GET {path} HTTP/1.1\r\n"
    request += f"Host: {host}\r\n"
    request += "Connection: close\r\n"
    request += "Accept: text/html,text/plain\r\n"
    request += "\r\n"
    return request

# downloads file from host and path, then writes
def download_file(host, path, output_file):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((host, 80))
            request = create_http_request(host, path)
            s.sendall(request.encode())
            response = b""
            while True:
                chunk = s.recv(4096)
                if not chunk:
                    break
                response += chunk
        header, body = response.split(b"\r\n\r\n", 1)
        header = header.decode()
        if "HTTP/1.1 200 OK" not in header:
            print(f"Error: {header.splitlines()[0]}")
            return
        with open(output_file, 'wb') as f:
            f.write(body)
        print(f"Downloading {host}{path} to {output_file}...SUCCESS.")
    except Exception as e:
        print(f"An error occurred: {e}")

def main():
    if len(sys.argv) != 3:
        print("Usage: http_downloader <URL> <output_file>")
        return
    url = sys.argv[1]
    output_file = sys.argv[2]
    host, path = parse_url(url)
    download_file(host, path, output_file)

if __name__ == "__main__":
    main()
