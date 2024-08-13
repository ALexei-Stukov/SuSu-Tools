local params = {...}

local file_name = params[1]
local accept_type = params[2]

local function read_file(filename,accept_type)
    local file = io.input(filename)
    local final_type = ""
 
    local read_type
    --text
    --html or css or javascript
    if string.match(accept_type,"/*.html") then
        final_type = "text/html"
        read_type = "text"
    end
    if string.match(accept_type,"/*.css") then 
        final_type = "text/css"
        read_type = "text"
    end
    if string.match(accept_type,"/*.js") then 
        final_type = "text/javascript"
        read_type = "text"
    end

    --image
    if string.match(accept_type,"image/*") then 
        final_type = "image/*"
        read_type = "image"
    end

    if file == nil or accept_type == nil then
        io.write("error")
    else
        io.write("HTTP/1.1 200 OK\r\n")
        io.write("Connection: keep-alive\r\n")
        io.write("Content-Type: "..final_type.."\r\n")
        io.write("Transfer-Encoding: chunked\r\n")
        io.write("\r\n")

        local line = ""
        while(true)do
            line = io.read(1024)      --逐KB读取html和图片，文件结束时返回nil
            if nil == line then
                break
            end
            local n = string.len(line)
            io.write(string.format("%x",n).."\r\n"..line.."\r\n")
        end

    io.close(file)
    io.write("0".."\r\n".."\r\n")
    end  
end
--[[
if accept_type == nil then  --若想下载二进制文件，则Accept 的值必须为空
    read_binary_file(file_name)
else
--]]

read_file(file_name,accept_type)
--[[
if string.match(accept_type,"text/") or string.match(file_name,"/*.js") then 
    read_file(file_name,accept_type)
elseif string.match(accept_type,"image/") then
    read_image_file(file_name,accept_type)
elseif string.match(accept_type,"application/octet") and string.match(accept_type,"stream") then    --可能是一个bug，当遇到AAA-BBB这样的字符串，string.match将失效，返回nil,迫不得已，使用分开识别的方法
--string.match(accept_type,"application/octet") 这样也许更好
end
--]]

----以上的代码就可以处理文本和图片了，但是在下载文件时还是会遇到问题。
----如果文件名是中文，那么在url里面，文件名是以 %xxx 的格式存在的，并不是明文的中文。
----所以还需要把url语言转换为utf-8语言，这样Lua脚本才能在操作系统内找到该文件。

--utf-8转成url
local function urlEncode(s)
    s = string.gsub(s, "([^%w%.%- ])", function(c)
        return string.format("%%%02X", string.byte(c))
    end)
    return string.gsub(s, " ", "+")
end

--url转成utf-8
local function urlDecode(s)
    s = string.gsub(s, "%%(%x%x)", function(h)
        return string.char(tonumber(h, 16))
    end)
    return s
end

--测试一个字符串是不是中文
local function testchinese(str)
    if string.match(str,"%%(%x%x)") then
        return true
    else
        return false
    end
end
