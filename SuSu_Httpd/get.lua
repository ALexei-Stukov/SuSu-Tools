local params = {...}

local file_name = params[1]
local accept_type = params[2]

--获得文件后缀
local function get_filename_tail(filename)
    --print(filename)
    local index = #filename
    for index = #filename, 1 , -1 do
        --print("index = "..index)
        --print(string.sub(filename, index,index))
        if string.sub(filename, index,index) == '.' then
            return string.sub(filename, index+1)
        end
    end
    return filename
end
--获得文件名,不包含后缀
local function get_filename(filename)
    --print(filename)
    local index = #filename
    local str_begin
    local str_end
    for index = #filename, 1 , -1 do
        --print("index = "..index)
        --print(string.sub(filename, index,index))
        if string.sub(filename, index,index) == '.' then
            str_end = index
        end
        if string.sub(filename, index,index) == '/' then
            str_begin = index
            return string.sub(filename, str_begin+1,str_end-1)
        end
    end
end


local chinese_flag 
--测试一个字符串是不是中文
local function testchinese(str)
    
    if string.match(str,"%%(%x%x)") then
        return true
    else
        return false
    end
    --[[local result = false
    for i = 1, string.len(val) do
        if string.byte(string.sub(val,i,i)) > 127 then
            return true
        else 
            result = false
        end 
    end 
    return result
    --]]
end

--utf-8转成url
function urlEncode(s)
    s = string.gsub(s, "([^%w%.%- ])", function(c)
        return string.format("%%%02X", string.byte(c))
    end)
    return string.gsub(s, " ", "+")
end
--url转成utf-8
function urlDecode(s)
    s = string.gsub(s, "%%(%x%x)", function(h)
        return string.char(tonumber(h, 16))
    end)
    return s
end

--读取文本
local function read_text_file(filename,text_type)
    local file = io.input(filename)
    local final_type = ""
    
    --text
    --html or css or javascript
    if string.match(text_type,"/*.html") then 
        final_type = "text/html"
    end
    if string.match(text_type,"/*.css") then 
        final_type = "text/css"
    end
    if string.match(text_type,"/*.js") then 
        final_type = "text/javascript"
    end

    if file == nil or text_type == nil then
        io.write("error")
    else
        io.write("HTTP/1.1 200 OK\r\n")
        io.write("Connection: keep-alive\r\n")
        io.write("Content-Type: "..final_type.."\r\n")
        io.write("Transfer-Encoding: chunked\r\n")
        io.write("\r\n")

        local line = ""
        while(true)do
            line = io.read()      --逐行读取内容，文件结束时返回nil
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

--读取图片
local function read_image_file(filename,text_type)
    local file = io.input(filename)
    local final_type = "image/*" --告知浏览器是图片

    if file == nil or text_type == nil then
        io.write("error")
    else
        io.write("HTTP/1.1 200 OK\r\n")
        io.write("Connection: keep-alive\r\n")
        io.write("Content-Type: "..final_type.."\r\n")
        io.write("Transfer-Encoding: chunked\r\n")
        io.write("\r\n")

        local line = ""
        while(true)do
            line = io.read(1024)      --逐KB读取html，文件结束时返回nil
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

--读取二进制
local function read_binary_file(filename)
    local file = io.input(filename)
    local file_name = get_filename(filename)--文件名,不包含后缀
    if chinese_flag then
        file_name = urlEncode(file_name)
    end
    local file_tail = get_filename_tail(filename)--文件名的后缀
    local final_type = "application/"..file_tail   --告知浏览器是字节流
                                        --其实就下载图片而言，final_type 设置为 字节流octet-stream 或者是图片images/*都没问题
                                        --因为浏览器以字节流形式下载了文件之后，如果能展示这个文件，比如文件本身就是图片，那么它就会展示。
                                        --但是，如果需要下载文件，则必须使用字节流。
    --print(file_name .. "." ..file_tail)
    if file == nil then
        io.write("error")
    else
        io.write("HTTP/1.1 200 OK\r\n")
        io.write("Connection: keep-alive\r\n")
        io.write("Content-Type: "..final_type.."\r\n")
        io.write("Content-Disposition: ".."attachment;filename="..file_name.."."..file_tail.."\r\n")
        --io.write("Accept-Length: ".."1234".."\r\n")
        io.write("Transfer-Encoding: chunked\r\n")
        io.write("\r\n")

        local line = ""
        while(true)do
            line = io.read(1024)      --逐KB读取html，文件结束时返回nil
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

--先判断params[1] 即url 是文件或文件夹
    --url只允许是这样的形式 
    --./root_path/   指向根路径
    --./root_path/SUSU_HTTPD    指向一个子路径
    --./root_path/SUSU_HTTPD/    指向一个子路径
    --./root_path/SUSU_HTTPD/file.xxx    指向一个文件  这里要求文件必须要有后缀

if string.sub(file_name,string.len(file_name)) == "/" then
    --带/的文件夹
    file_name = file_name.."index.html"
elseif string.match(file_name,"/%..*%..*") then --他妈的，这个正则有够烦。 .任意字符(1次或多次).任意字符(1次或多次)
    --文件
    local check = io.open(file_name,r)
    if(check ~= nil) then
        io.close(check)
    else
        --发生了错误，文件不存在,输出错误的html
    end
else
    --不带/的文件夹
    file_name = file_name.."/".."index.html"
end


if(check ~= nil) then
    io.close(check)
end
--]]


--检查一下文件名是否包含中文,若包含，把全局标记设置为1，在适当时转码
chinese_flag = testchinese(file_name)

--utf-8转成url
--urlEncode(s)

--url转成utf-8
if chinese_flag then
    file_name = urlDecode(file_name)
end

--判断一下文件类型  文本 图片 二进制   并分别执行不同的读取函数
--print(file_name.." "..accept_type)
--print(file_name)
--file_name = urlEncode(file_name)
--print(file_name)

--js请求的accept_type可能是 */* ,捕捉accept_type 无法获得有效信息，要捕捉url

    if accept_type == nil then  --若想下载二进制文件，则Accept 的值必须为空
        read_binary_file(file_name)
    elseif string.match(accept_type,"text/") or string.match(file_name,"/*.js") then 
        read_text_file(file_name,accept_type)
    elseif string.match(accept_type,"image/") then
        read_image_file(file_name,accept_type)
    elseif string.match(accept_type,"application/octet") and string.match(accept_type,"stream") then    --可能是一个bug，当遇到AAA-BBB这样的字符串，string.match将失效，返回nil,迫不得已，使用分开识别的方法
    --string.match(accept_type,"application/octet") 这样也许更好

    end