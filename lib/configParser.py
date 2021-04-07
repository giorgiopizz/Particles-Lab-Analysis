import configparser
from sys import argv
import os,sys,inspect
current_dir = os.path.dirname(os.path.abspath(inspect.getfile(inspect.currentframe())))
parent_dir = os.path.dirname(current_dir)
lib_dir = parent_dir + "/lib"

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

    if 'fit.cfg' in os.listdir():
        configFilename = parent_dir+'/fit.cfg'

        config.read(configFilename)


        config['general']['name'] = configName

        config[configName] = dict(config['example']).copy()
        config[configName]['filename'] = file
        config[configName]['title'] = plotTitle

        with open(configFilename, 'w') as configfile:
            config.write(configfile)
    else:
        print("can't find fit.cfg")



if __name__ == "__main__":
    if len(argv)<=1:
        print('error')
    else:
        configName = '_'.join(argv[1].split('_')[1:])

        l = ['up', 'down','tot']
        for suffix in l:
            cfgName = configName + '_' + suffix
            createConfig(cfgName, os.path.join(parent_dir, 'tempi', argv[1]+'_'+ suffix +'.txt'), 'Carbon ' + suffix.capitalize() + ' Decay')


    # config = configparser.ConfigParser()
    # config.read('../fit.cfg')
    #
    # print(config.sections())
    # for key in config['general']:
    #     print(key)
    #     print(config['general'][key])
    #
    # print(dict(config['general']))
    # createConfig('prova', 'cioaiodi.txt', 'Io')
    #
    # with open('../fit.cfg', 'w') as configfile:
    #     config.write(configfile)
