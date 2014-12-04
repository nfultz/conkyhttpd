do

    print("Loading webconky...")
    local myopen = package.loadlib("./webconky.so", "luaopen_webconky")
    myopen()
    webconky.setup()

    function conky_listen()
        webconky.listen()
        return ""
    end


    function conky_close()
        webconky.close()
        return ""
    end

end
