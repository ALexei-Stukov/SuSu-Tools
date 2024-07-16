# SuSu-Tools
Some Tiny and useful tools base on c++11.

**Very friendly to beginners**


# Short describe about this project

This project is mainly to pay off "technical debt".

In order to practice my skills, I wrote my own **httpd** after I graduated from school.

But I was too eager for quick success,That makes **the code became extremely confusing**. (Don’t be impatient in coding)

So I have to **refactor** my httpd. 

# The goal of this project

Building a **high-performance http service**. (Maybe also include **https** service)

# Project structure

The project includes the following tools:

[Timer](http://url.com/ "Title")

[Epoll](http://url.com/ "Title")

[InitParam](http://url.com/ "Title")

[ThreadPool](http://url.com/ "Title")

[NetProtocol](http://url.com/ "Title")

These tools make up the following program:

[Httpd](http://url.com/ "Title")

# How to use this project
## Httpd
```
sudo apt install lua5.3     # Lua is the default script,But you can change it to other script.
make httpd          # the http service
make run            # run the http service
```
Then open **127.0.0.1:9512** in browser to view the default web page.

**9512** is the **default port**, you can change the port to **80** or **8080** in the **config file**.

## the tools
If you wan to **use the tools**, just check the **example folder** or the **test folder**.

# How to make some contribution
Just use it.
