from xmlrpc.server import SimpleXMLRPCServer

def add(x,y):
    return x+y
def subtract(x,y):
    return x-y
def mul(x,y):
    return x*y

server = SimpleXMLRPCServer(("localhost",8000))
print("Listening on port 8000.....")

server.register_function(add,"add")
server.register_function(subtract,"subtract")
server.register_function(mul,"mul")

server.serve_forever()