
import socket

serv_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM, proto=0)
serv_sock.bind(('127.0.0.1', 1200))
serv_sock.listen(5)


#easy number 8605947675529404139874810840481068082583242194608026978477629364060344809259056845617161578672540742799116325488504866110633755335384379329654901772823274951102440324964947061751802214401296284484343848313621531662980288085335598181989893152275817625035543110239476570867650914853295013736820367857701
#            392325365435903005563769329007

def publick():
    print("hello")


def private():
    return "dir"


def work(data):
    '''
    n=336741257
    d=265577951
    strin=str(data)
    print(strin)
    strin=strin[3:-1]
    print("<<>>")
    print(strin)
    strin=strin.split(" ")
    glob_mes=""
    for i in range(0,len(strin)):
        lock_mes=int(strin[i])
        lock_mes=int(pow(lock_mes,d,n))
        unlock_mes=str(lock_mes)
        if len(unlock_mes)<10:
            unlock_mes="0"+unlock_mes
        glob_mes+=unlock_mes
    print(glob_mes)
    str_converted = ""
    octal_str=[glob_mes[i:i + 3] for i in range(0, len(glob_mes), 3)]
    '''
    print(data)
    data=data.split("\n")
    win_ver=data[17].split(":")
    if win_ver[0]=="[WIN_VER]" and win_ver[1]=="Windows8OrGreater":
        return "back"
    #for octal_char in octal_str:
      #  str_converted += chr(int(octal_char, 8))
    #print( str_converted)


flag=True
while flag:
    # Бесконечно обрабатываем входящие подключения
    choose=""
    if choose=="back":
            b=private().encode('utf-8')
            client_sock.sendall(b)
    client_sock, client_addr = serv_sock.accept()
    print('Connected by', client_addr)
    client_port=client_addr[1]
    client_ip=client_addr[0]
    while True:
        if choose=="back":
             break
        # Пока клиент не отключился, читаем передаваемые
        # им данные и отправляем их обратно
        data = client_sock.recv(2048)
        #print(data)
        if not data:
            # Клиент отключился
            break
        if data:
            choose=work(data.decode('utf-8'))
        elif data==b'Closed':
            flag=False
            break


    client_sock.close()