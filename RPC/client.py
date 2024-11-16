import xmlrpc.client

server_url = "http://localhost:8000"

client = xmlrpc.client.ServerProxy(server_url)

x = int(input("Enter the value of x :"))
y = int(input("Enter the value of y :"))

add_result = client.add(x,y)
subtract_result = client.subtract(x,y)
mul_result = client.mul(x,y)

print(f"{x} + {y} = {add_result}")
print(f"{x} - {y} = {subtract_result}")
print(f"{x} * {y} = {mul_result}")