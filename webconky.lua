do

    print("Loading conkyhttpd module...")
    require("conkyhttpd")

    function conky_setup(port)
        conkyhttpd.setup(port)
        return ""
    end

    function conky_cleanup()
        conkyhttpd.cleanup()
        return ""
    end

    function conky_listen()
        local i =  conky_info["update_interval"]
        conkyhttpd.listen(i)
        return ""
    end


end
