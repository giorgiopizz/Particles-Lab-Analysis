import configparser

def createConfig (configName, file, plotTitle):
    """ retrieves the config file in the parent folder,
    adds a section with the configName specified
    and writes it to the config file. It also updates the general
    name
    """

    # filename=../tempi/
    # title = Carbon Up Decay
    # save = 0
    # f_type= 1
    # parameters = 144, 2.1, 10
    # range = 0.4, 11
    # bin= 100
    # search=1
    config = configparser.ConfigParser()
    config.read('../fit.cfg')

    config['general']['name'] = configName

    configObj[configName] = dict(configObj['example']).copy()
    configObj[configName]['filename'] = file
    configObj[configName]['title'] = plotTitle

    with open('../fit.cfg', 'w') as configfile:
        config.write(configfile)



if __name__ == "__main__":
    config = configparser.ConfigParser()
    config.read('../fit.cfg')

    print(config.sections())
    for key in config['general']:
        print(key)
        print(config['general'][key])

    print(dict(config['general']))
    createConfig('prova', 'cioaiodi.txt', 'Io')

    with open('../fit.cfg', 'w') as configfile:
        config.write(configfile)
