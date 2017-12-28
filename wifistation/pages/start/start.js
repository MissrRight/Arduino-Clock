// start.js

Page({

  /**
   * 页面的初始数据
   */
  data: {
    loading:false,
    opacity: 0.4,
    disabled: true,
    threshold: 0,
    rule: 'up',
    direction: 0,
    items: [
      { name: 'up', value: '高于门限报警' ,checked:'ture'},
      { name: 'down', value: '低于门限报警' },
    ]
  },

  onReady: function () {
    var that = this
    wx.onCompassChange(function (res) {
      that.setData({
        direction: parseInt(res.direction)
      })
    })
  },
//跳转页面
datapos:function(){
  const requestTask = wx.navigateTo({
    url: '../navigate/navigate',
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
  //onReady是在页面初次加载时调用且只会被调用一次
  //页面加载完成后播放背景音乐
  onReady: function () {
    //调用接口进行背景音乐的播放
   wx.playBackgroundAudio({
     dataUrl: 'http://oursparkspace.cn/wp-content/uploads/2017/12/Keren-Ann-Right-Now-Right-Here.mp3',//网络音乐的链接
     title:'反正很好听',//自己给这个音乐起个名字
     coverImgUrl:'https://timgsa.baidu.com/timg?image&quality=80&size=b9999_10000&sec=1513614706821&di=7aa47c08577a223411c21d70c5c18af8&imgtype=0&src=http%3A%2F%2Fwww.ifenglife.com%2Feditor%2Fattached%2Fimage%2F20161207%2F20161207154507_4950.jpg'//给音乐当封面的图片的URL
   })

  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
  
  
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
