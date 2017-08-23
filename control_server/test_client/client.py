import socket, sys

send_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
recv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

host = sys.argv[1]

send_port = int(sys.argv[2])  # server port
recv_port = int(sys.argv[3])

send_sock.connect((host, send_port))
recv_sock.connect((host, recv_port))


while(1):# get letter
	k = input('enter a letter:')
	send_sock.send(k.encode())  # send k to server
	# if stop signal, then leave loop
	if k == 'exit': break
	v = recv_sock.recv(1024).decode()  # receive v from server (up to 1024 bytes)
	print(v)

send_sock.close()
print('send socket closed') # close socket
recv_sock.close()
print('recv socket closed')