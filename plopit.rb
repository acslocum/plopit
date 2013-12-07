require 'rubygems'
require 'serialport'

#java -classpath classes:lib/log4j/log4j-api-2.0-beta3.jar:lib/log4j/log4j-core-2.0-beta3.jar fortress.CommandLine 
#| while read line ; do echo $line | say ; done

def run
  serial = SerialPort.new('/dev/tty.usbmodemfa131',9600,8,1,SerialPort::NONE)
  files = Dir.glob("sounds/*.mp3")
  puts files.join(",")
  while(true) do
    line = serial.readline("\n")
    play_sound(line.chomp)
  end
end

def play_sound(line)
  files = Dir.glob("sounds/#{line}*.mp3")
  sample = files.sample
  puts "#{line} #{sample}"
  `afplay #{sample}` unless files.empty?
  
end


run
