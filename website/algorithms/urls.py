#from django.conf.urls import url
from django.urls import path
from . import views

app_name = 'algorithms'

urlpatterns = [
    #url(r'^$', views.home, name='home'),

    path('', views.home, name='home'),

    path('algorithms/', views.AlgorithmListView.as_view(), name='index'),

    path('algorithms/<pk>/', views.DetailView.as_view(), name='detail'),

    path('register/', views.UserFormView.as_view(), name='register'),

    path('login/', views.LoginView.as_view(), name='login'),

    path('reg-success/', views.wait, name='wait'),

    path('add-algorithm/', views.AddAlgorithm.as_view(), name='add-alg'),

    path('logout/', views.logoutView, name='logout'),

    path('compare/', views.CompAlgView.as_view(), name='comp'),
    # error dla fun 3 dim 10

    path('ranking/', views.RankingView.as_view(), name='ranking'),

    path('algorithms/<pk>/failed/', views.delete, name='delete'),

    path('benchmarks/', views.BenchmarksView.as_view(), name='benchmark'),
]
