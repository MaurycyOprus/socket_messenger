HOST="127.0.0.1"
PORT=1100
import socket
from tkinter import *
import os
import time

# GUI zrobione na szablonie: https://www.simplifiedpython.net/python-gui-login/

def register():
    global register_screen
    register_screen = Toplevel(main_screen)
    register_screen.title("Register")
    register_screen.geometry("300x250")
 
    global username
    global password
    global username_entry
    global password_entry
    username = StringVar()
    password = StringVar()
 
    Label(register_screen, text="Please enter details below", bg="blue").pack()
    Label(register_screen, text="").pack()
    username_lable = Label(register_screen, text="Username * ")
    username_lable.pack()
    username_entry = Entry(register_screen, textvariable=username)
    username_entry.pack()
    password_lable = Label(register_screen, text="Password * ")
    password_lable.pack()
    password_entry = Entry(register_screen, textvariable=password, show='*')
    password_entry.pack()
    Label(register_screen, text="").pack()
    Button(register_screen, text="Register", width=10, height=1, bg="blue", command = register_user).pack()

def register_user():

    username_info = username.get()
    password_info = password.get()

    msg_from_server = ""
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s:
        s.connect((HOST,PORT))
        while True:
            time.sleep(1)
            registration = "yes"
            s.send(bytes(registration, "utf-8"))
            time.sleep(1)
            s.send(bytes(username_info,"utf-8"))
            time.sleep(1)
            s.send(bytes(password_info,"utf-8"))
            data=s.recv(1024)
            msg_from_server = data.decode("utf-8")
            print(msg_from_server)
            s.close()
            break

    username_entry.delete(0, END)
    password_entry.delete(0, END)

    time.sleep(1)
    if(str(msg_from_server[0:24]) == str("Registration Successful.")):
        Label(register_screen, text="Registration Success", fg="green", font=("calibri", 11)).pack()
    else:
        Label(register_screen, text="Username already taken!", fg="red", font=("calibri", 11)).pack()
    msg_from_server = ""

def login():
    global login_screen
    login_screen = Toplevel(main_screen)
    login_screen.title("Login")
    login_screen.geometry("300x250")
    Label(login_screen, text="Please enter details below to login").pack()
    Label(login_screen, text="").pack()
 
    global username_verify
    global password_verify
 
    username_verify = StringVar()
    password_verify = StringVar()
 
    global username_login_entry
    global password_login_entry
 
    Label(login_screen, text="Username * ").pack()
    username_login_entry = Entry(login_screen, textvariable=username_verify)
    username_login_entry.pack()
    Label(login_screen, text="").pack()
    Label(login_screen, text="Password * ").pack()
    password_login_entry = Entry(login_screen, textvariable=password_verify, show= '*')
    password_login_entry.pack()
    Label(login_screen, text="").pack()
    Button(login_screen, text="Login", width=10, height=1, command = login_verify).pack()

def login_verify():
    username_info = username_verify.get()
    password_info = password_verify.get()

    msg_from_server = ""
    with socket.socket(socket.AF_INET,socket.SOCK_STREAM) as s:
        s.connect((HOST,PORT))
        while True:
            time.sleep(1)
            registration = "no"
            s.send(bytes(registration, "utf-8"))
            time.sleep(1)
            s.send(bytes(username_info,"utf-8"))
            time.sleep(1)
            s.send(bytes(password_info,"utf-8"))
            data=s.recv(1024)
            msg_from_server = data.decode("utf-8")
            print(msg_from_server)
            s.close()
            break

    username_login_entry.delete(0, END)
    password_login_entry.delete(0, END)

    time.sleep(1)
    if(msg_from_server[0:17] == "Login successful."):
        Label(login_screen, text="Login Success", fg="green", font=("calibri", 11)).pack()
        global splash
        splash = Tk()
        splash.geometry("500x500")
        splash.title("Messenger")
        Button(splash, text="Friends list", height="2", width="30", command = Friends_list).pack()
        Label(splash, text="").pack()
        Button(splash, text="Add friend", height="2", width="30", command=add_friend).pack()
        Label(splash, text="").pack()
        Button(splash, text="Friends requests", height="2", width="30", command=friend_request).pack()
    else:
        Label(login_screen, text="Wrong username or password!", fg="red", font=("calibri", 11)).pack()
    msg_from_server = ""

def Friends_list():
    global Friends_list_screen
    Friends_list_screen = Toplevel(splash)
    Friends_list_screen.geometry("300x250")
    Friends_list_screen.title("Friends list")
    Label(Friends_list_screen, text="Click to chat", width="300", height="2", font=("Calibri", 13)).pack()
    Label(Friends_list_screen, text="").pack()
    Button(Friends_list_screen, text="Friend1", height="2", width="30", command = Friends_list).pack()
    Label(Friends_list_screen, text="").pack()
    Button(Friends_list_screen, text="Friend2", height="2", width="30", command=Friends_list).pack()
    Label(Friends_list_screen, text="").pack()
    Button(Friends_list_screen, text="Friend3", height="2", width="30", command=Friends_list).pack()

def add_friend():
    global add_friend_screen
    nickname = StringVar()
    add_friend_screen = Toplevel(splash)
    add_friend_screen.geometry("300x250")
    add_friend_screen.title("Add friend")
    Label(add_friend_screen, text="Enter friend's nickname", bg="blue", width="300", height="2", font=("Calibri", 13)).pack()
    Label(add_friend_screen, text="").pack()
    nickname_entry = Entry(add_friend_screen, textvariable=nickname)
    nickname_entry.pack()
    Label(add_friend_screen, text="").pack()
    Button(add_friend_screen, text="Add", height="2", width="30", command=add_friend_destroy).pack()

def add_friend_destroy():
    #wyślij zaproszenie do znajomych na serwer
    add_friend_screen.destroy()

def friend_request():
    global friend_request_screen
    nickname = StringVar()
    friend_request_screen = Toplevel(splash)
    friend_request_screen.geometry("300x250")
    friend_request_screen.title("Friend requests")
 

def main_account_screen():
    global main_screen
    main_screen = Tk()
    main_screen.geometry("300x250")
    main_screen.title("Account Login")
    Label(text="Select Your Choice", bg="blue", width="300", height="2", font=("Calibri", 13)).pack()
    Label(text="").pack()
    Button(text="Login", height="2", width="30", command = login).pack()
    Label(text="").pack()
    Button(text="Register", height="2", width="30", command=register).pack()

    main_screen.mainloop()