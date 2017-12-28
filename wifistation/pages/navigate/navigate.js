// pages/navigate/navigate.js
var app = getApp()
Page({

  /**
   * 页面的初始数据
   */
  data: {
    changdata:12,
    tem:null,
    temperature:666
  },

  getDataFromOneNet: function () {
    //从oneNET请求我们的Wi-Fi气象站的数据
    const requestTask = wx.request({
      url: 'https://api.heclouds.com/devices/20426624/datapoints?datastream_id=Light,Temperature,Humidity&limit=15',
      header: {
        'content-type': 'application/json',
        'api-key': '1RPD03nLPWrEVYbhC=r994bFctE='
      },
      success: function (res) {
        //拿到数据后保存到全局数据
        var app = getApp()
        app.globalData.temperature = res.data.data.datastreams[0]
        app.globalData.light = res.data.data.datastreams[1]
        app.globalData.humidity = res.data.data.datastreams[2]
        console.log(app.globalData.light)
        //跳转到天气页面，根据拿到的数据绘图
        wx.navigateTo({
          url: '../wifi_station/tianqi/tianqi',
        })
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },

  /**
   * 生命周期函数--监听页面加载
   */
  onLoad: function (options) {
  
  },

  /**
   * 生命周期函数--监听页面初次渲染完成
   */
  onReady: function () {

  
  },

  /**
   * 生命周期函数--监听页面显示
   */
  //onshow是页面显示时调用 可被调用多次
  onShow: function () {
    //发起请求
     const requestTask = wx.request({
       //？前为服务器接口地址 ？后照着onenet官方文档 调用了温度的数据 因为只想看到实时数据 每次只传一个设置limit=1
      url: 'https://api.heclouds.com/devices/20426624/datapoints?datastream_id=Temperature&limit=1',
      //发送报文
      header: {
        'content-type': 'application/json',
        'api-key': '1RPD03nLPWrEVYbhC=r994bFctE='
      },
      //数据请求成功 执行success函数 
      success: (res) => {
        console.log(res.data)
        console.log(app.globalData.light)
        //用在data中定义好的变量绑定传输过来的数据
        this.setData({ "temperature": res.data.data.datastreams[0].datapoints})
        console.log(res.data.data.datastreams[0])
        tem = app.globalData.temperature
        console.log(app.globalData.temperature)
      },

      fail: function (res) {
        console.log("fail!!!")
      },

      complete: function (res) {
        console.log("end")
      }
    })
  },
  /**
   * 生命周期函数--监听页面隐藏
   */
  onHide: function () {
  
  },

  /**
   * 生命周期函数--监听页面卸载
   */
  onUnload: function () {
  
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {

  },

  /**
   * 页面上拉触底事件的处理函数
   */
  onReachBottom: function () {
  
  },

  /**
   * 用户点击右上角分享
   */
  onShareAppMessage: function () {
  
  }
})