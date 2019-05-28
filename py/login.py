import hashlib
import os
import re
from lxml import etree
from urllib.parse import urljoin
from urllib import parse, request
base_header = {
    "Accept": "*/*",
    "Accept-Language": "zh-CN",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko",
    "Content-Type": "application/x-www-form-urlencoded",
    "Accept-Encoding": "gzip, deflate",
    "Connection": "Keep-Alive",
    "Pragma": "no-cache"
}


class Encrypt:
    def __init__(self, schoolNum, user='', pwd='', vcode=''):
        self.pwd = pwd
        self.user = user
        self.vcode = vcode
        self.school = schoolNum
        pass

    @staticmethod
    def md5(obj):
        md5 = hashlib.md5(obj.encode('gb2312')).hexdigest()
        return md5

    def EnPwd(self, pwd='', user=''):
        pwd = pwd or self.pwd
        if not pwd:
            raise Exception('No Pwd Found')
        user = user or self.user
        if not pwd:
            raise Exception('No User Found')
        dsdsdsdsdxcxdfgfg = self.md5(
            user + self.md5(pwd)[:30].upper() + self.school)[:30].upper()  # upper转大写而已，根据js写
        return dsdsdsdsdxcxdfgfg

    def Enverfy(self, vcode=''):
        vcode = vcode or self.vcode
        if not vcode:
            raise Exception('No VerfyImageCode Found')
        fgfggfdgtyuuyyuuckjg = self.md5(
            self.md5(vcode.upper())[:30].upper() + self.school)[:30].upper()  # upper转大写而已，根据js写
        return fgfggfdgtyuuyyuuckjg


class Login:
    def __init__(self, url, ip):  # 初始化时先获取当前session会话的Cookie跟获取动态VIEWSTATE的内容
        self.url = url
        base_header['Host'] = ip
        self.GetViewstate()

    def GetViewstate(self):  # 设置Cookie跟获取session内容，这里可以不用refer，不知道为什么
        url = urljoin(self.url, './_data/index_LOGIN.aspx')
        req = request.Request(url, headers=base_header)
        res = request.urlopen(req)
        Cookie = res.headers['Set-Cookie']
        base_header['Cookie'] = Cookie[:Cookie.find(';')]
        print(res.headers)
        res = res.read()
        tree = etree.HTML(res)
        viewstate = tree.xpath(r"//input[@name='__VIEWSTATE']")[0].get('value')
        self._viewstate = viewstate
        return viewstate

    def getval(self):  # 下载验证码图片
        url = self.url+'sys/ValidateCode.aspx'
        head = base_header
        head['Referer'] = urljoin(self.url, './_data/index_LOGIN.aspx')
        # 一定要有refer，你不信可以注释掉，你会发现图片能下载，但是打验证码正确也没有。
        # 这个估计是开启了二次验证，不仅包括了cookie
        req = request.Request(url, headers=base_header)
        res = request.urlopen(req)
        print(res.headers)
        res = res.read()
        files = open('val.jpg', 'wb')
        files.write(res)
        files.close

    def dataappend(self, pw, code, id):  # post数据配置
        data = {}
        data["__VIEWSTATE"] = self._viewstate
        data["pcInfo"] = "Mozilla/5.0 (Windows NT 10.0; WOW64; rv:48.0) Gecko/20100101 Firefox/48.0Windows NT 10.0; WOW645.0 (Windows) SN:NULL"
        data["typeName"] = u"学生"
        data['dsdsdsdsdxcxdfgfg'] = pw
        data['fgfggfdgtyuuyyuuckjg'] = code
        data['Sel_Type'] = "STU"
        data["txt_asmcdefsddsd"] = id
        data["txt_pewerwedsdfsdff"] = ''
        data["txt_sdertfgsadscxcadsads"] = ""
        data["sbtState"] = ""
        return data

    def login(self, id, pw):  # 登录动作
        self.getval()  # 获取验证码
        code = input("请输入验证码")  # 输入验证码
        url = self.url+"_data/index_login.aspx"
        ec = Encrypt('10577', id, pw, code)
        pw = ec.EnPwd()
        code = ec.Enverfy()
        data = self.dataappend(pw, code, id)
        data = parse.urlencode(
            data, encoding='gb2312').encode(encoding='gb2312')
        print(data)
        head = base_header
        head['Referer'] = urljoin(self.url, './_data/index_LOGIN.aspx')
        req = request.Request(url=url, headers=base_header, method='POST',
                              data=data)
        res = request.urlopen(req)
        res = res.read()
        res = str(res, 'gb2312')
        if re.search(u"登录失败", res):
            return False
        elif re.search(u'正在加载权限', res):
            return True
        else:
            return False


class StusearchMsg:
    def __init__(self, school):
        if isinstance(school, str):
            self._school = school
        else:
            raise Exception('School is Not a Valid Type')
        pass

    def scoreMsg(self, searchmsg):
        data = self.SetPostData(searchmsg)
        data = parse.urlencode(
            data, encoding='gb2312').encode(encoding='gb2312')
        response = request.Request(urljoin(
            self._school, './xscj/Stu_MyScore_rpt.aspx'), data=data, headers=base_header)
        res = request.urlopen(response)
        res = res.read()
        res = str(res, 'gb2312')
        if re.search(u"惠州学院学生成绩明细", res):
            return self.getpatch(res)
        else:
            print(res)
            return False, "登录过期"

    def SetPostData(self, searchmsg):
        if isinstance(searchmsg, dict) == False:
            return False
        try:
            xn = searchmsg['xn']
            sj = searchmsg['sj']
            zfx = searchmsg['zfx']
        except Exception as e:
            raise Exception('without mes as %s' % (e))

        '''
        :sel_xn 学期学年
        :SJ 0原始 1有效
        :btn_serarch 选择检索还是打印
        :SelXNXQ 入学以来0 学年1 学期2
        :zfx_falg 主修0 辅修1
        :zxf废弃 
        '''
        postdata = {}
        postdata['sel_xn'] = xn
        postdata['SJ'] = sj
        postdata['btn_search'] = u"检索".encode("gb2312")
        postdata['SelXNXQ'] = '1'
        postdata['zfx_falg'] = zfx
        # postdata['zxf']='0'
        print(postdata)
        return postdata

    def getpatch(self, txt, path=os.getcwd()):
        htmltxt = txt
        url = ''
        headers = {
            "Referer": urljoin(self._school, 'xscj/Stu_MyScore_rpt.aspx'),
        }
        headers = dict(base_header, **headers)
        start = htmltxt.find("src='Stu_MyScore_Drawimg.aspx")
        htmltxt = htmltxt[start+len("src='"):]
        htmltxt = htmltxt[:htmltxt.find("'")]
        url = "./xscj/"+htmltxt
        url = urljoin(self._school, url)
        req = request.Request(url, headers=headers)
        res = request.urlopen(req)
        img = res.read()
        if os.path.isdir(path):
            f = open(os.path.join(path, 'Score.jpg'), 'wb')
            f.write(img)
            f.close()
        return "success"


if __name__ == "__main__":
    login = Login("http://202.192.224.155/jwweb/", '202.192.224.155')
    id = 'xxxx'
    pw = 'xxxx'
    if id == 'xxxx':
        exit(0)
    searchmsg = {}
    searchmsg['xn'] = '2017'  # 获取17年，有效，主修成绩
    searchmsg['sj'] = '1'
    searchmsg['zfx'] = '0'
    if login.login(id, pw):
        score = StusearchMsg("http://202.192.224.155/jwweb/")
        print(score.scoreMsg(searchmsg=searchmsg))
